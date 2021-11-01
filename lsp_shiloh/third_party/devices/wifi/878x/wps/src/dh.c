/** @file  dh.c 
  * @brief This file contains function for Diffie-Hellman Key
  * 
  * Copyright (C) 2003-2008, Marvell International Ltd.
  * All Rights Reserved
  */

#include "includes.h"
#include "common.h"
#include "dh.h"
#include "tx_api.h"

/********************************************************
        Local Functions
********************************************************/
static void
string_to_u32(u32 * digit, u32 no_digits, u8 * string, u32 no_chars)
{
    u32 tmp;
    int j;
    u32 i, u;

    for (i = 0, j = no_chars - 1; i < no_digits && j >= 0; i++) {
        tmp = 0;
        for (u = 0; j >= 0 && u < U32_BITS_LENGTH; j--, u += 8)
            tmp |= ((u32) string[j]) << u;
        digit[i] = tmp;
    }

    for (; i < no_digits; i++)
        digit[i] = 0;
}

static void
u32_to_string(u8 * string, u32 no_chars, u32 * digit, u32 no_digit)
{
    u32 tmp;
    int j;
    unsigned int i, u;

    for (i = 0, j = no_chars - 1; i < no_digit && j >= 0; i++) {
        tmp = digit[i];
        for (u = 0; j >= 0 && u < U32_BITS_LENGTH; j--, u += 8)
            string[j] = (unsigned char) (tmp >> u);
    }

    for (; j >= 0; j--)
        string[j] = 0;
}

static void
digit_divide(u32 * result, u32 * dividend, u32 divisor)
{
    u16 result_hi_16bits, result_lo_16bits;
    u16 divisor_hi_16bits, divisor_lo_16bits;
    u32 hi_32bits, lo_32bits;
    u32 msb_16, divisor_lo_hi_part;
    u32 lo_lo, lo_hi;

    divisor_hi_16bits = HIGH_16_BITS(divisor);
    divisor_lo_16bits = LOW_16_BITS(divisor);
    hi_32bits = dividend[1];
    lo_32bits = dividend[0];

    if (divisor_hi_16bits == 0xFFFF)
        result_hi_16bits = HIGH_16_BITS(hi_32bits);
    else
        result_hi_16bits = hi_32bits / (divisor_hi_16bits + 1);

    lo_hi = result_hi_16bits * divisor_lo_16bits;
    msb_16 = RIGHT_SHIFT_16_BITS(lo_hi);

    if ((lo_32bits -= msb_16) > (0xFFFFFFFF - msb_16))
        hi_32bits--;

    hi_32bits -= ((result_hi_16bits * divisor_hi_16bits) + HIGH_16_BITS(lo_hi));

    divisor_lo_hi_part = RIGHT_SHIFT_16_BITS(divisor_lo_16bits);

    while ((hi_32bits > divisor_hi_16bits) ||
           ((hi_32bits == divisor_hi_16bits) &&
            (lo_32bits >= divisor_lo_hi_part))) {
        if ((lo_32bits -=
             divisor_lo_hi_part) > (0xFFFFFFFF - divisor_lo_hi_part))
            hi_32bits--;
        hi_32bits -= divisor_hi_16bits;
        result_hi_16bits++;
    }

    if (divisor_hi_16bits == 0xFFFF)
        result_lo_16bits = LOW_16_BITS(hi_32bits);
    else
        result_lo_16bits =
            (RIGHT_SHIFT_16_BITS(hi_32bits) +
             HIGH_16_BITS(lo_32bits)) / (divisor_hi_16bits + 1);

    lo_lo = result_lo_16bits * divisor_lo_16bits;
    lo_hi = result_lo_16bits * divisor_hi_16bits;

    if ((lo_32bits -= lo_lo) > (0xFFFFFFFF - lo_lo))
        hi_32bits--;

    if ((lo_32bits -=
         RIGHT_SHIFT_16_BITS(lo_hi)) >
        (0xFFFFFFFF - RIGHT_SHIFT_16_BITS(lo_hi)))
        hi_32bits--;

    hi_32bits -= HIGH_16_BITS(lo_hi);

    while ((hi_32bits > 0) || (!hi_32bits && lo_32bits >= divisor)) {
        if ((lo_32bits -= divisor) > (0xFFFFFFFF - divisor))
            hi_32bits--;
        result_lo_16bits++;
    }

    *result = RIGHT_SHIFT_16_BITS(result_hi_16bits) + result_lo_16bits;
}

static void
digit_multiply(u32 * result, u32 multiplicand, u32 multiplier)
{
    u32 result1, result2, carry, tmp;

    result[0] = LOW_16_BITS(multiplicand);
    result[0] *= LOW_16_BITS(multiplier);
    result[1] = HIGH_16_BITS(multiplicand);
    result[1] *= HIGH_16_BITS(multiplier);
    result1 = LOW_16_BITS(multiplicand);
    result1 *= HIGH_16_BITS(multiplier);
    result2 = LOW_16_BITS(multiplier);
    result2 *= HIGH_16_BITS(multiplicand);
    carry = LOW_16_BITS(result1) + LOW_16_BITS(result2);
    if (carry & 0x00010000)
        result[1]++;
    tmp = result[0];
    result[0] += RIGHT_SHIFT_16_BITS(LOW_16_BITS(carry));
    if (result[0] < tmp)
        result[1]++;
    carry = HIGH_16_BITS(result1) + HIGH_16_BITS(result2);
    result[1] += carry;
}

static int
digit_array_most_significant_digit_position(u32 * array, u32 size)
{
    int i;

    for (i = (int) (size - 1); i >= 0; i--) {
        if (array[i])
            break;
    }

    return (i + 1);
}

static u32
find_digit_msb(u32 digit)
{
    u32 i;

    for (i = 0; i < U32_BITS_LENGTH; i++)
        if (!(digit >> 1))
            break;

    return (i);
}

static int
digit_array_compare(u32 * n, u32 * m, u32 size)
{
    int i;

    for (i = (int) (size - 1); i >= 0; i--) {
        if (n[i] > m[i])
            return (1);
        if (n[i] < m[i])
            return (-1);
    }

    return (0);
}

static void
digit_array_multiply(u32 * result, u32 * array_multiplicand,
                     u32 * array_multiplier, u32 digit_array_size)
{
    int array_multiplicand_position, array_multiplier_position;
    int i, j;
    u32 carry, tmp[2];

    array_multiplicand_position =
        digit_array_most_significant_digit_position(array_multiplicand,
                                                    digit_array_size);
    array_multiplier_position =
        digit_array_most_significant_digit_position(array_multiplier,
                                                    digit_array_size);

    memset(result, 0, 2 * digit_array_size * sizeof(u32));
    for (i = 0; i < array_multiplicand_position; i++) {
        carry = 0;
        for (j = 0; j < array_multiplier_position; j++) {
            digit_multiply(tmp, array_multiplicand[i], array_multiplier[j]);
            result[i + j] += carry;
            if (result[i + j] < carry)
                carry = 1;
            else
                carry = 0;
            result[i + j] += tmp[0];
            if (result[i + j] < tmp[0])
                carry++;
            carry += tmp[1];
        }
        result[i + j] += carry;
    }
}

static void
digit_array_quotient_and_remainder(u32 * quotient, u32 * remainder,
                                   u32 * dividend_array, u32 dividend_size,
                                   u32 * divisor_array, u32 divisor_size)
{
    u32 divisor_msd, msb;
    static u32 p[2 * MAX_DIGIT_SIZE], q[2 * MAX_DIGIT_SIZE];
    u32 quot, divisor;
    u32 shift;
    u32 borrow, carry, tmp[2];
    int i, j;

    divisor_msd =
        digit_array_most_significant_digit_position(divisor_array,
                                                    divisor_size);
    memset(quotient, 0, dividend_size * sizeof(u32));
    memset(remainder, 0, divisor_size * sizeof(u32));
    msb = find_digit_msb(divisor_array[divisor_msd - 1]);
    shift = 32 - msb;
    carry = 0;

    for (i = 0; i < dividend_size; i++) {
        q[i] = (dividend_array[i] << shift) | carry;
        carry = shift ? (dividend_array[i] >> msb) : 0;
    }
    q[i + 1] = carry;

    carry = 0;
    for (i = 0; i < divisor_size; i++) {
        p[i] = (divisor_array[i] << shift) | carry;
        carry = shift ? (divisor_array[i] >> msb) : 0;
    }

    divisor = divisor_array[divisor_msd - 1] + 1;

    for (i = (int) (dividend_size - divisor_msd); i >= 0; i--) {
        if (divisor == 0)
            quot = q[i + divisor_msd];
        else
            digit_divide(&quot, &q[i + divisor_msd - 1], divisor);

        borrow = 0;
        for (j = 0; j < divisor_msd; j++) {
            digit_multiply(tmp, quot, p[j]);

            if ((q[i + j] -= borrow) > (0xFFFFFFFF - borrow))
                borrow = 1;
            else
                borrow = 0;

            if ((q[i + j] -= tmp[0]) > (0xFFFFFFFF - tmp[0]))
                borrow++;

            borrow += tmp[1];
        }

        q[i + divisor_msd] -= borrow;

        while (q[i + divisor_msd] ||
               (digit_array_compare(&q[i], p, divisor_msd) >= 0)) {
            quot++;
            borrow = 0;

            for (j = 0; j < divisor_msd; j++) {
                tmp[0] = q[i + j] - borrow;

                if (tmp[0] > (0xFFFFFFFF - borrow))
                    tmp[0] = 0xFFFFFFFF - p[j];
                else if ((tmp[0] -= p[j]) > (0xFFFFFFFF - p[j]))
                    borrow = 1;
                else
                    borrow = 0;

                q[i + j] = tmp[0];
            }
            q[i + divisor_msd] -= borrow;
        }                       /* end of while loop */
        quotient[i] = quot;
    }                           /* end of i loop */

    carry = 0;
    for (i = (int) (divisor_msd - 1); i >= 0; i--) {
        remainder[i] = (q[i] << shift) | carry;
        carry = shift ? (remainder[i] >> msb) : 0;
    }

    /* Zeroize potentially sensitive information. */
    memset(p, 0, sizeof(p));
    memset(q, 0, sizeof(q));
}

static void
digit_array_multiply_and_modulo(u32 * result, u32 * multiplicand,
                                u32 * multiplier, u32 * divisor_array,
                                u32 array_size)
{
    static u32 digit[2 * MAX_DIGIT_SIZE], quotient[2 * MAX_DIGIT_SIZE];

    digit_array_multiply(digit, multiplicand, multiplier, array_size);
    digit_array_quotient_and_remainder(quotient, result, digit,
                                       array_size * 2, divisor_array,
                                       array_size);

    /* Zeroize potentially sensitive information. */
    memset(digit, 0, sizeof(digit));
    memset(quotient, 0, sizeof(quotient));
}

static void
digit_array_exponent_and_modulo(u32 * result, u32 * base, u32 * exponent_array,
                                u32 exponent_size, u32 * divisor_array,
                                u32 divisor_size)
{
    static u32 power[3][2 * MAX_DIGIT_SIZE], tmp[2 * MAX_DIGIT_SIZE];
    u32 exponent_msd, exponent, bits;
    u32 j, index;
    int i;

    ENTER();

    memcpy((void *) power[0], (void *) base, sizeof(u32) * divisor_size);
    digit_array_multiply_and_modulo(power[1], power[0], base,
                                    divisor_array, divisor_size);
    digit_array_multiply_and_modulo(power[2], power[1], base,
                                    divisor_array, divisor_size);

    memset(tmp, 0, sizeof(tmp));
    tmp[0] = 1;

    exponent_msd =
        digit_array_most_significant_digit_position(exponent_array,
                                                    exponent_size);

    for (i = (int) (exponent_msd - 1); i >= 0; i--) {
        bits = U32_BITS_LENGTH;
        exponent = exponent_array[i];

        /* Skip leading zero bits of most significant digit. */
        if (i == (int) (exponent_msd - 1)) {
            while (!GET_2MSB(exponent)) {
                exponent <<= 2;
                bits -= 2;
            }
        }

        for (j = 0; j < bits; j += 2, exponent <<= 2) {
            digit_array_multiply_and_modulo(tmp, tmp, tmp,
                                            divisor_array, divisor_size);

            digit_array_multiply_and_modulo(tmp, tmp, tmp,
                                            divisor_array, divisor_size);

            if ((index = GET_2MSB(exponent)) != 0)
                digit_array_multiply_and_modulo(tmp, tmp, power[index - 1],
                                                divisor_array, divisor_size);
        }
    }

    memcpy((void *) result, (void *) tmp, (sizeof(u32) * divisor_size));

    /* Zeroize potentially sensitive information. */
    memset(tmp, 0, sizeof(tmp));
    memset(power, 0, sizeof(power));
    LEAVE();
}

static int
generate_random_bytes(u8 * block, u32 blockLen)
{
    u32 available = 0;
    //struct timeval tv;
    //u32 ut;

    ENTER();

    srand(tx_time_get());

    while (blockLen > available)
        block[available++] = rand() % 0x100;

    LEAVE();
    return (0);
}

/********************************************************
        Global Functions
********************************************************/
/** 
 *  @brief  Sets up Diffie-Hellman key agreement.
 *
 *  @param publicKey        Pointers to public key generated
 *  @param privateKey       Pointers to private key generated randomly
 *  @param privateKeyLen    Length of private key
 *  @param params           Parameters for DH algorithm
 *  @return                 0 on success, -1 on failure
 */
int
setup_dh_agreement(u8 * publicKey, 
                   u32 public_len,
                   u8 * privateKey, u32 privateKeyLen, DH_PG_PARAMS * params)
{
    static u32 base[MAX_DIGIT_SIZE], prime[MAX_DIGIT_SIZE],
        private_key[MAX_DIGIT_SIZE];
    static u32 public_key[MAX_DIGIT_SIZE];
    u32 prime_digit_pos, private_key_pos;

    ENTER();

    /* Zeroize potentially sensitive information. */
    memset(base, 0, sizeof(base));
    memset(prime, 0, sizeof(prime));
    memset(private_key, 0, sizeof(private_key));
    memset(public_key, 0, sizeof(public_key));

    string_to_u32(prime, MAX_DIGIT_SIZE, params->prime, params->primeLen);
    prime_digit_pos =
        digit_array_most_significant_digit_position(prime, MAX_DIGIT_SIZE);
    string_to_u32(base, MAX_DIGIT_SIZE, params->generator,
                  params->generatorLen);

    if (generate_random_bytes(privateKey, privateKeyLen) != 0)
        return (-1);

    string_to_u32(private_key, MAX_DIGIT_SIZE, privateKey, privateKeyLen);
    private_key_pos =
        digit_array_most_significant_digit_position(private_key,
                                                    MAX_DIGIT_SIZE);

    digit_array_exponent_and_modulo(public_key, base,
                                    private_key, private_key_pos, prime,
                                    prime_digit_pos);

    u32_to_string(publicKey, params->primeLen, public_key, prime_digit_pos);

    LEAVE();
    return (0);
}

/** 
 *  @brief  Computes agreed key from the other party's public value,
 *          private value, and Diffie-Hellman parameters.
 *
 *  @param agreedKey        Pointers to agreed key generated
 *  @param otherPublicValue Other party's public key
 *  @param privateKey       Private key
 *  @param privateKeyLen    Lengths of private key
 *  @param params           Parameters for DH algorithm
 *  @return                 0 on success, -1 on failure
 */
int
compute_dh_agreed_key(u8 * agreedKey, 
                      u8 * otherPublicValue, 
                      u32 public_len,
                      u8 * privateKey,
                      u32 privateKeyLen, DH_PG_PARAMS * params)
{
    static u32 prime[MAX_DIGIT_SIZE], private_key[MAX_DIGIT_SIZE],
        public_key[MAX_DIGIT_SIZE];
    static u32 agree_key[MAX_DIGIT_SIZE];
    u32 prime_digit_pos, private_key_pos;

    ENTER();

    /* Zeroize potentially sensitive information. */
    memset(prime, 0, sizeof(prime));
    memset(private_key, 0, sizeof(private_key));
    memset(public_key, 0, sizeof(public_key));
    memset(agree_key, 0, sizeof(agree_key));

    string_to_u32(prime, MAX_DIGIT_SIZE, params->prime, params->primeLen);
    prime_digit_pos =
        digit_array_most_significant_digit_position(prime, MAX_DIGIT_SIZE);
    string_to_u32(private_key, MAX_DIGIT_SIZE, privateKey, privateKeyLen);

    private_key_pos =
        digit_array_most_significant_digit_position(private_key,
                                                    MAX_DIGIT_SIZE);
    string_to_u32(public_key, MAX_DIGIT_SIZE, otherPublicValue,
                  params->primeLen);

    if (digit_array_compare(public_key, prime, prime_digit_pos) >= 0)
        return (-1);

    digit_array_exponent_and_modulo(agree_key, public_key,
                                    private_key, private_key_pos, prime,
                                    prime_digit_pos);

    u32_to_string(agreedKey, params->primeLen, agree_key, prime_digit_pos);

    LEAVE();
    return (0);
}

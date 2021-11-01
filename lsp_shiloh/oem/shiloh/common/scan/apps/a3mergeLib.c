#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "a3mergeLib.h"
#include "logger.h"

#define DBG_PRFX "SCN MergeLib: "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(1)
#define DBG_PRINTF_SDH_ERR(...) DBG_PRINTF(LOG_SDH_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_PRINTF_SDH_DEBUG(...) DBG_PRINTF(LOG_SDH_DEBUG, DBG_PRFX __VA_ARGS__)

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

extern int a3_dlt_height;	//for dlt - a3 height. add.jong.2012-08-14

BOOL make2Dpseudo_floatarray(Pseudo_float2Dgray *array, int width, int height)
{
	int i;
	array->height = height;
	array->width = width;
	array->data = (pseudo_float **)calloc(height, sizeof(pseudo_float *));
	if(array->data == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
		return FALSE;
	}

	array->data[0] = (pseudo_float *)calloc(height * width, sizeof(pseudo_float));
	if(array->data[0] == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
		return FALSE;
	}

	for(i = 1; i < height; i++)
	{
		array->data[i] = array->data[0] + width * i;
	}

	return TRUE;
}

BOOL make2Dfloatarray(Float2Dgray *array, int width, int height)
{
	int i;
	array->height = height;
	array->width = width;
	array->data = (float **)calloc(height, sizeof(float *));
	if(array->data == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
		return FALSE;
	}

	array->data[0] = (float *)calloc(height * width, sizeof(float));
	if(array->data[0] == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, (height * width));
		return FALSE;
	}

	for(i = 1; i < height; i++)
	{
		array->data[i] = array->data[0] + width * i;
	}

	return TRUE;
}

BOOL make2DBYTEarray(BYTE2Dgray *array, int width, int height)
{
	int i;
	array->height = height;
	array->width = width;
	array->data = (unsigned char **)calloc(height, sizeof(unsigned char *));
	if(array->data == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
		return FALSE;
	}

	array->data[0] = (unsigned char *)calloc(height * width, sizeof(unsigned char));
	if(array->data[0] == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, (height * width));
		return FALSE;
	}

	for(i = 1; i < height; i++)
	{
		array->data[i] = array->data[0] + width * i;
	}

	return TRUE;
}

BOOL make2DBYTEarrayfrom1Darray(unsigned char *src, BYTE2Dgray *dst, int width, int height)
{
	int i;
	dst->height = height;
	dst->width = width;
	dst->data = (unsigned char **)calloc(height, sizeof(unsigned char *));
	if(dst->data == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
		return FALSE;
	}

	dst->data[0] = src;

	for(i = 1; i < height; i++)
	{
		dst->data[i] = dst->data[0] + width * i;
	}

	return TRUE;
}

BOOL make2Dchararray(Char2Dgray *array, int width, int height)
{
	int i;
	array->height = height;
	array->width = width;
	array->data = (char **)calloc(height, sizeof(char *));
	if(array->data == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
		return FALSE;
	}

	array->data[0] = (char *)calloc(height * width, sizeof(char));
	if(array->data[0] == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, (height * width));
		return FALSE;
	}

	for(i = 1; i < height; i++)
	{
		array->data[i] = array->data[0] + width * i;
	}

	return TRUE;
}

BOOL make2DUINTarray(UINT2Dgray *array, int width, int height)
{
	int i;
	array->height = height;
	array->width = width;
	array->data = (UINT **)calloc(height, sizeof(UINT *));
	if(array->data == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
		return FALSE;
	}

	array->data[0] = (UINT *)calloc(height * width, sizeof(UINT));
	if(array->data[0] == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, (height * width));
		return FALSE;
	}

	for(i = 1; i < height; i++)
	{
		array->data[i] = array->data[0] + width * i;
	}

	return TRUE;
}

BOOL make2Dshortarray(Short2Dgray *array, int width, int height)
{
	int i;
	array->height = height;
	array->width = width;
	array->data = (short **)calloc(height, sizeof(short *));
	if(array->data == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
		return FALSE;
	}

	array->data[0] = (short *)calloc(height * width, sizeof(short));
	if(array->data[0] == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, (height * width));
		return FALSE;
	}

	for(i = 1; i < height; i++)
	{
		array->data[i] = array->data[0] + width * i;
	}

	return TRUE;
}

BOOL make2Dintarray(Int2Dgray *array, int width, int height)
{
	int i;
	array->height = height;
	array->width = width;
	array->data = (int **)calloc(height, sizeof(int *));
	if(array->data == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
		return FALSE;
	}

	array->data[0] = (int *)calloc(height * width, sizeof(int));
	if(array->data[0] == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, (height * width));
		return FALSE;
	}

	for(i = 1; i < height; i++)
	{
		array->data[i] = array->data[0] + width * i;
	}

	return TRUE;
}

BOOL make2Dcolorarray(BYTE2Dcolor *array, int width, int height)
{
	int i;
	array->height = height;
	array->width = width;
	array->data = (RgbPixel **)calloc(height, sizeof(RgbPixel *));
	if(array->data == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
		return FALSE;
	}

	array->data[0] = (RgbPixel *)calloc(height * width, sizeof(RgbPixel));
	if(array->data[0] == NULL)
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, (height * width));
		return FALSE;
	}

	for(i = 1; i < height; i++)
	{
		array->data[i] = array->data[0] + width * i;
	}

	return TRUE;
}

void setZerofloat(Float2Dgray *array)
{
	memset(array->data[0], 0, array->height * array->width * sizeof(float));
}

void deletefloat2D(Float2Dgray *array)
{
	if(array->data != NULL)
	{
		free(array->data[0]);
		free(array->data);
	}
}

void setZeropseudo_float(Pseudo_float2Dgray *array)
{
	memset(array->data[0], 0, array->height * array->width * sizeof(pseudo_float));
}

void deletepseudo_float2D(Pseudo_float2Dgray *array)
{
	if(array->data != NULL)
	{
		free(array->data[0]);
		free(array->data);
	}
}

void deleteBYTE2D(BYTE2Dgray *array)
{
	if(array->data != NULL)
	{
		free(array->data[0]);
		free(array->data);
	}
}

void deletecolor2D(BYTE2Dcolor *array)
{
	if(array->data != NULL)
	{
		free(array->data[0]);
		free(array->data);
	}
}

void deleteint2D(Int2Dgray *array)
{
	if(array->data != NULL)
	{
		free(array->data[0]);
		free(array->data);
	}
}

void deleteshort2D(Short2Dgray *array)
{
	if(array->data != NULL)
	{
		free(array->data[0]);
		free(array->data);
	}
}

void conv2(Float2Dgray *in_img, Float2Dgray *out_img, Float2Dgray *mask)
{
	int i, j, m, n;
	int height = out_img->height;
	int width = out_img->width;
	int mask_width = mask->width;
	int mask_height = mask->height;
	int half_mask_width = (int)(mask_width / 2);
	int half_mask_height = (int)(mask_height / 2);
	int height_range = height - half_mask_height;
	int width_range = width - half_mask_width;
	float var;

	if(mask_width % 2 == 0 || mask_height % 2 == 0)
	{
		DBG_PRINTF_SDH_DEBUG("conv2(): Mask size must be odd!\n");
		return;
	}

#if FAST_CONV
	memset(out_img->data[0], 0, out_img->width * out_img->height * sizeof(float));
#endif
	for(i = half_mask_height; i < height_range; i++)
	{
		for(j = half_mask_width; j < width_range; j++)
		{
			var = 0.0;
			for(m = -half_mask_height; m <= half_mask_height; m++)
			{
				for(n = -half_mask_width; n <= half_mask_width; n++)
				{
					var += in_img->data[(m + i)][n + j] * mask->data[(m + half_mask_height)][n + half_mask_width];
				}
			}

			out_img->data[i][j] = var;
		}
	}

#if FAST_CONV
#else
	//1
	for(i = 0; i < half_mask_height; i++)
	{
		for(j = 0; j < width; j++)
		{
			var = 0.0;
			for(m = -half_mask_height; m <= half_mask_height; m++)
			{
				for(n = -half_mask_width; n <= half_mask_width; n++)
				{
					if(m + i < 0)
					{
						p = -(m + i);
					}
					else
					{
						p = m + i;
					}

					if(n + j < 0)
					{
						q = -(n + j);
					}
					else if(n + j >= width)
					{
						q = 2 * width - (n + j) - 2;
					}
					else
					{
						q = n + j;
					}

					var += in_img->data[p][q] * mask->data[(m + half_mask_height)][n + half_mask_width];
				}
			}

			out_img->data[i][j] = var;
		}
	}

	//2
	for(i = height_range; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			var = 0.0;
			for(m = -half_mask_height; m <= half_mask_height; m++)
			{
				for(n = -half_mask_width; n <= half_mask_width; n++)
				{
					if(m + i >= height)
					{
						p = 2 * height - (m + i) - 2;
					}
					else
					{
						p = m + i;
					}

					if(n + j < 0)
					{
						q = -(n + j);
					}
					else if(n + j >= width)
					{
						q = 2 * width - (n + j) - 2;
					}
					else
					{
						q = n + j;
					}

					var += in_img->data[p][q] * mask->data[(m + half_mask_height)][n + half_mask_width];
				}
			}

			out_img->data[i][j] = var;
		}
	}

	//3
	for(i = half_mask_height; i < height_range; i++)
	{
		for(j = 0; j < half_mask_width; j++)
		{
			var = 0.0;
			for(m = -half_mask_height; m <= half_mask_height; m++)
			{
				for(n = -half_mask_width; n <= half_mask_width; n++)
				{
					p = m + i;
					if(n + j < 0)
					{
						q = -(n + j);
					}
					else
					{
						q = n + j;
					}

					var += in_img->data[p][q] * mask->data[(m + half_mask_height)][n + half_mask_width];
				}
			}

			out_img->data[i][j] = var;
		}
	}

	//4
	for(i = half_mask_height; i < height_range; i++)
	{
		for(j = width_range; j < width; j++)
		{
			var = 0.0;
			for(m = -half_mask_height; m <= half_mask_height; m++)
			{
				for(n = -half_mask_width; n <= half_mask_width; n++)
				{
					p = m + i;
					if(n + j >= width)
					{
						q = 2 * width - (n + j) - 2;
					}
					else
					{
						q = n + j;
					}

					var += in_img->data[p][q] * mask->data[(m + half_mask_height)][n + half_mask_width];
				}
			}

			out_img->data[i][j] = var;
		}
	}
#endif
}

int gaussian(Float2Dgray *input, Float2Dgray *output, float sigma)
{
	int i, j;
	int length, center;
	Float2Dgray mask;
	float sum = 0;
	if(sigma < 0.0)
	{
		DBG_PRINTF_SDH_ERR("gaussian(): Value of Sigma must be larger than zero!\n");
		return FALSE;
	}

	length = (int)(6 * sigma) / 2 * 2 + 1;
	center = length / 2;

	if(FALSE == make2Dfloatarray(&mask, length, length))
	{
		return MEMORY_FAIL;
	}

	for(i = 0; i < length; i++)
	{
		for(j = 0; j < length; j++)
		{
			mask.data[i][j] = 1.0f / (2.0f * PI * sigma * sigma) * (float)exp(-(((i - center) * (i - center) + (j - center) * (j - center)) / (2.0f * sigma * sigma)));
			sum += mask.data[i][j];
		}
	}

	for(i = 0; i < length; i++)
	{
		for(j = 0; j < length; j++)
		{
			mask.data[i][j] /= sum;
		}
	}

	conv2(input, output, &mask);
	deletefloat2D(&mask);

	return FALSE;
}

int matMul(Float2Dgray *srcA, Float2Dgray *srcB, Float2Dgray *dst, BOOL srcA_t, BOOL srcB_t)
{
	Float2Dgray tmp_src;
	Float2Dgray *tmp_dst = &tmp_src;
	int i, j, k;
	float val;
	if(srcA == dst)
	{
		if(FALSE == make2Dfloatarray(tmp_dst, srcA->width, srcA->height))
		{
			return MEMORY_FAIL;
		}
	}
	else if(srcB == dst)
	{
		if(FALSE == make2Dfloatarray(tmp_dst, srcB->width, srcB->height))
		{
			return MEMORY_FAIL;
		}
	}
	else
	{
		tmp_dst = dst;
	}

	if(srcA_t &!srcB_t)
	{
		for(i = 0; i < tmp_dst->height; i++)
		{
			for(j = 0; j < tmp_dst->width; j++)
			{
				val = 0;
				for(k = 0; k < srcA->height; k++)
				{
					val += srcA->data[k][i] * srcB->data[k][j];
				}

				tmp_dst->data[i][j] = val;
			}
		}
	}
	else if((!srcA_t) & srcB_t)
	{
		for(i = 0; i < tmp_dst->height; i++)
		{
			for(j = 0; j < tmp_dst->width; j++)
			{
				val = 0;
				for(k = 0; k < srcA->width; k++)
				{
					val += srcA->data[i][k] * srcB->data[j][k];
				}

				tmp_dst->data[i][j] = val;
			}
		}
	}
	else if(srcA_t & srcB_t)
	{
		for(i = 0; i < tmp_dst->height; i++)
		{
			for(j = 0; j < tmp_dst->width; j++)
			{
				val = 0;
				for(k = 0; k < srcA->height; k++)
				{
					val += srcA->data[k][i] * srcB->data[j][k];
				}

				tmp_dst->data[i][j] = val;
			}
		}
	}
	else
	{
		for(i = 0; i < tmp_dst->height; i++)
		{
			for(j = 0; j < tmp_dst->width; j++)
			{
				val = 0;
				for(k = 0; k < srcA->width; k++)
				{
					val += srcA->data[i][k] * srcB->data[k][j];
				}

				tmp_dst->data[i][j] = val;
			}
		}
	}

	if(srcA == dst)
	{
		deletefloat2D(srcA);
		srcA->data = tmp_dst->data;
		srcA->data[0] = tmp_dst->data[0];
	}
	else if(srcB == dst)
	{
		deletefloat2D(srcB);
		srcB->data = tmp_dst->data;
		srcB->data[0] = tmp_dst->data[0];
	}

	return FALSE;
}

BOOL invert(Float2Dgray *in_mat)
{
	int width = in_mat->width;
	int height = in_mat->height;
	int i, j, k;
	float sum;
	float **data = in_mat->data;
	if(width != height)
	{
		return FALSE;
	}

	if(width == 1)
	{
		return FALSE;	// must be of dimension >= 2
	}

	for(i = 1; i < width; i++)
	{
		data[0][i] /= data[0][0];	// normalize row 0
	}

	for(i = 1; i < width; i++)
	{
		for(j = i; j < width; j++)
		{	// do a column of L
			sum = 0.0f;
			for(k = 0; k < i; k++)
			{
				sum += data[j][k] * data[k][i];
			}

			data[j][i] -= sum;
		}

		if(i == width - 1)
		{
			continue;
		}

		for(j = i + 1; j < width; j++)
		{	// do a row of U
			sum = 0.0f;
			for(k = 0; k < i; k++)
			{
				sum += data[i][k] * data[k][j];
			}

			data[i][j] = (data[i][j] - sum) / data[i][i];
		}
	}

	for(i = 0; i < width; i++)
	{	// invert L
		for(j = i; j < width; j++)
		{
			float x = 1.0f;
			if(i != j)
			{
				x = 0.0f;
				for(k = i; k < j; k++)
				{
					x -= data[j][k] * data[k][i];
				}
			}

			data[j][i] = x / data[j][j];
		}
	}

	for(i = 0; i < width; i++)
	{	// invert U
		for(j = i; j < width; j++)
		{
			if(i == j)
			{
				continue;
			}

			sum = 0.0f;
			for(k = i; k < j; k++)
			{
				sum += data[k][j] * ((i == k) ? 1.0f : data[i][k]);
			}

			data[i][j] = -sum;
		}
	}

	for(i = 0; i < width; i++)
	{	// final inversion
		for(j = 0; j < width; j++)
		{
			sum = 0.0f;
			for(k = ((i > j) ? i : j); k < width; k++)
			{
				sum += ((j == k) ? 1.0f : data[j][k]) * data[k][i];
			}

			data[j][i] = sum;
		}
	}

	return TRUE;
}

int paste(BYTE2Dgray *img1, BYTE2Dgray *dst, int sx, int sy)
{
	int nw = img1->width;
	int nh = img1->height;

	int vert, horz;
	for(vert = 0; vert < nh; vert++)
	{
		for(horz = 0; horz < nw; horz++)
		{
			dst->data[(vert + sy)][horz + sx] = img1->data[vert][horz];
		}
	}

	return FALSE;
}

float linear_itp_float(Float2Dgray *img, float x, float y)
{
	int up_y = ceil(y);
	int bo_y = floor(y);
	int up_x = ceil(x);
	int bo_x = floor(x);

	//if(x < 0 || x > img->width - 1 || y < 0 || y > img->height - 1)
	//	return 255;
	float **imgdata = img->data;
	float out = imgdata[bo_y][bo_x] * ((float)up_y - y) * ((float)up_x - x) +
		imgdata[bo_y][up_x] * ((float)up_y - y) * (x - (float)bo_x) +
		imgdata[up_y][bo_x] * (y - (float)bo_y) * ((float)up_x - x) +
		imgdata[up_y][up_x] * (y - (float)bo_y) * (x - (float)bo_x);
	return out;
}

unsigned char linear_itp_BYTE(BYTE2Dgray *img, pseudo_float x, pseudo_float y)
{
	int up_y = pseudo_ceil(y);
	int bo_y = pseudo_floor(y);
	int up_x = pseudo_ceil(x);
	int bo_x = pseudo_floor(x);
	unsigned char **imgdata = img->data;

	pseudo_float f_up_y = up_y * one;
	pseudo_float f_bo_y = bo_y * one;
	pseudo_float f_up_x = up_x * one;
	pseudo_float f_bo_x = bo_x * one;

	//if(x < 0 || x > img->width - 1 || y < 0 || y > img->height - 1)
	//	return 255;
	pseudo_float out = (pseudo_float) imgdata[bo_y][bo_x] * (f_up_y - y) * (f_up_x - x) +
		(pseudo_float) imgdata[bo_y][up_x] * (f_up_y - y) * (x - f_bo_x) +
		(pseudo_float) imgdata[up_y][bo_x] * (y - f_bo_y) * (f_up_x - x) +
		(pseudo_float) imgdata[up_y][up_x] * (y - f_bo_y) * (x - f_bo_x);
#if FLOAT_ROUND
	out = out + a_half2;
	out /= one2;
#else
	out /= one2;
#endif
	return(unsigned char)out;
}

int resize(BYTE2Dgray *img, int scale)
{
#if RESIZE_QUALITY
	int new_width = (img->width + scale / 2) / scale;
	int new_height = (img->height + scale / 2) / scale;

	BYTE2Dgray tmp_img;
	int i, j;

	if(FALSE == make2DBYTEarray(&tmp_img, new_width, new_height))
	{
		return MEMORY_FAIL;
	}

	for(i = 0; i < new_height; i++)
	{
		//i_= (((float)i + 0.5) / (float)new_height) * (float)(img->height) - 0.5;
		pseudo_float i_ = (i * one + a_half) * scale - a_half;
		for(j = 0; j < new_width; j++)
		{
			//j_= (((float)j + 0.5) / (float)new_width) * (float)(img->width) - 0.5;
			pseudo_float j_ = (j * one + a_half) * scale - a_half;
			tmp_img.data[i][j] = linear_itp_BYTE(img, j_, i_);
		}
	}
#else
	unsigned char **imgdata = img->data;
	int new_width = img->width / scale;
	int new_height = img->height / scale;

	BYTE2Dgray tmp_img;
	int i, j;

	if(FALSE == make2DBYTEarray(&tmp_img, new_width, new_height))
	{
		return MEMORY_FAIL;
	}

	for(i = 0; i < new_height; i++)
	{
		int i_ = i * scale;
		for(j = 0; j < new_width; j++)
		{
			int j_ = j * scale;
			tmp_img.data[i][j] = imgdata[i_][j_];
		}
	}
#endif
	deleteBYTE2D(img);
	img->width = new_width;
	img->height = new_height;
	img->data = tmp_img.data;
	img->data[0] = tmp_img.data[0];

	return FALSE;
}

void circle(BYTE2Dgray *src, Point2D32f point, int radius, int thickness, unsigned char color)
{
	int widht = src->width;
	int height = src->height;
	int i, j_p, j_m, k;
	int x = round(point.x);
	int y = round(point.y);
	unsigned char **srcdata = src->data;
	thickness = min(radius, thickness);

	for(k = 0; k < thickness; k++)
	{
		for(i = -radius; i <= radius; i++)
		{
			if(i + x >= 0 && i + x < widht)
			{
				j_p = y + round(sqrt((float)radius * radius - i * i));
				j_m = y + y - j_p;

				if(j_p == j_m)
				{
					if(j_p >= 0 && j_p < height)
					{
						srcdata[j_p][i + x] = color;
					}

					j_p++;
					j_m--;
				}

				if(j_p >= 0 && j_p < height)
				{
					srcdata[j_p][i + x] = color;
				}

				if(j_m >= 0 && j_m < height)
				{
					srcdata[j_m][i + x] = color;
				}
			}
		}

		radius--;
	}
}

void line(BYTE2Dgray *src, Point2D32f sp, Point2D32f dp, int thickness, unsigned char color)
{
	int left_x, left_y, right_x, right_y;
	int i, j, k;
	float m;
	int width = src->width;
	int height = src->height;
	int count;
	float offset = (float)(thickness - 1) / 2.0f;
	unsigned char **srcdata = src->data;

	if(sp.x < dp.x)
	{
		left_x = round(sp.x);
		left_y = round(sp.y - offset);
		right_x = round(dp.x);
		right_y = round(dp.y - offset);
		m = (dp.y - sp.y) / (dp.x - sp.x);
	}
	else
	{
		left_x = round(dp.x);
		left_y = round(dp.y - offset);
		right_x = round(sp.x);
		right_y = round(sp.y - offset);
		m = (sp.y - dp.y) / (sp.x - dp.x);
	}

	for(k = 0; k < thickness; k++)
	{
		count = 0;
		for(i = left_x; i <= right_x; i++)
		{
			if(i >= 0 && i < width)
			{
				j = left_y + round(count * m);
				if(j >= 0 && j < height)
				{
					srcdata[j][i] = color;
				}
			}

			count++;
		}

		left_y++;
		right_y++;
	}
}

int imgWarpAffine(BYTE2Dgray *src, BYTE2Dgray *dst, Float2Dgray *tr)	//2.3.0 warp fix
{
	int width = src->width;
	int height = src->height;
	pseudo_float p_width = (width - 1) * one;
	pseudo_float p_height = (height - 1) * one;
	int i, j, out;

	float max_x = 0.0f, min_x = (float)dst->width, max_y = 0.0f, min_y = (float)dst->height;
	int i_max_x, i_min_x, i_max_y, i_min_y;
	Point2D32f tmp_p, tmp_p1, tmp_p2;
	float m, b;
	Float2Dgray tmp_rep_tr;
	Pseudo_float2Dgray rep_tr;
	unsigned char **srcdata, **dstdata;
	float **trdata = tr->data;

	if(tr->width != 3 || tr->height != 3)
	{
		return width - 1;
	}

	//make2Dfloatarray(&tmp, 1, 3);
	//compute size of warping place
	i = 0;
	j = 0;
	tmp_p.x = trdata[0][0] * i + trdata[0][1] * j + trdata[0][2];
	tmp_p.y = trdata[1][0] * i + trdata[1][1] * j + trdata[1][2];
	if(max_x < tmp_p.x)
	{
		max_x = tmp_p.x;
	}

	if(min_x > tmp_p.x)
	{
		min_x = tmp_p.x;
	}

	if(max_y < tmp_p.y)
	{
		max_y = tmp_p.y;
	}

	if(min_y > tmp_p.y)
	{
		min_y = tmp_p.y;
	}

	tmp_p1 = tmp_p;

	i = width - 1;
	tmp_p.x = trdata[0][0] * i + trdata[0][1] * j + trdata[0][2];
	tmp_p.y = trdata[1][0] * i + trdata[1][1] * j + trdata[1][2];
	if(max_x < tmp_p.x)
	{
		max_x = tmp_p.x;
	}

	if(min_x > tmp_p.x)
	{
		min_x = tmp_p.x;
	}

	if(max_y < tmp_p.y)
	{
		max_y = tmp_p.y;
	}

	if(min_y > tmp_p.y)
	{
		min_y = tmp_p.y;
	}

	i = 0;
	j = height - 1;
	tmp_p.x = trdata[0][0] * i + trdata[0][1] * j + trdata[0][2];
	tmp_p.y = trdata[1][0] * i + trdata[1][1] * j + trdata[1][2];
	if(max_x < tmp_p.x)
	{
		max_x = tmp_p.x;
	}

	if(min_x > tmp_p.x)
	{
		min_x = tmp_p.x;
	}

	if(max_y < tmp_p.y)
	{
		max_y = tmp_p.y;
	}

	if(min_y > tmp_p.y)
	{
		min_y = tmp_p.y;
	}

	tmp_p2 = tmp_p;

	i = width - 1;
	tmp_p.x = trdata[0][0] * i + trdata[0][1] * j + trdata[0][2];
	tmp_p.y = trdata[1][0] * i + trdata[1][1] * j + trdata[1][2];
	if(max_x < tmp_p.x)
	{
		max_x = tmp_p.x;
	}

	if(min_x > tmp_p.x)
	{
		min_x = tmp_p.x;
	}

	if(max_y < tmp_p.y)
	{
		max_y = tmp_p.y;
	}

	if(min_y > tmp_p.y)
	{
		min_y = tmp_p.y;
	}

	if(tmp_p1.y < 0 || tmp_p2.y < 0 || tmp_p1.y > height - 1 || tmp_p2.y > height - 1)
	{
		m = (tmp_p1.x - tmp_p2.x) / (tmp_p1.y - tmp_p2.y);
		b = tmp_p1.x - m * tmp_p1.y;

		out = ceil(m * (float)(height - 1) + b);
		out = max(ceil(b), out);
	}
	else
	{
		out = ceil(max(tmp_p1.x, tmp_p2.x));
	}

	i_min_x = max(floor(min_x), 0);

	if(i_min_x < trdata[0][2])
	{
		i_min_x = (int)trdata[0][2];	//2.2.0 bug fix
	}

	i_min_y = max(floor(min_y), 0);
	i_max_x = min(ceil(max_x), dst->width);
	i_max_y = min(ceil(max_y), dst->height);

	//Rect.x = min_x;
	//Rect.y = min_y;
	//Rect.width = max_x - min_x + 1;
	//Rect.height = max_y - min_y + 1;
	if(FALSE == make2Dfloatarray(&tmp_rep_tr, 3, 3))
	{
		return MEMORY_FAIL;
	}

	tmp_rep_tr.data[0][0] = trdata[0][0];
	tmp_rep_tr.data[0][1] = trdata[0][1];
	tmp_rep_tr.data[0][2] = trdata[0][2];
	tmp_rep_tr.data[1][0] = trdata[1][0];
	tmp_rep_tr.data[1][1] = trdata[1][1];
	tmp_rep_tr.data[1][2] = trdata[1][2];
	tmp_rep_tr.data[2][0] = trdata[2][0];
	tmp_rep_tr.data[2][1] = trdata[2][1];
	tmp_rep_tr.data[2][2] = trdata[2][2];
	invert(&tmp_rep_tr);

	if(FALSE == make2Dpseudo_floatarray(&rep_tr, 3, 3))
	{
		return MEMORY_FAIL;
	}

	rep_tr.data[0][0] = myround(tmp_rep_tr.data[0][0] * one);
	rep_tr.data[0][1] = myround(tmp_rep_tr.data[0][1] * one);
	rep_tr.data[0][2] = myround(tmp_rep_tr.data[0][2] * one);
	rep_tr.data[1][0] = myround(tmp_rep_tr.data[1][0] * one);
	rep_tr.data[1][1] = myround(tmp_rep_tr.data[1][1] * one);
	rep_tr.data[1][2] = myround(tmp_rep_tr.data[1][2] * one);
	rep_tr.data[2][0] = myround(tmp_rep_tr.data[2][0] * one);
	rep_tr.data[2][1] = myround(tmp_rep_tr.data[2][1] * one);
	rep_tr.data[2][2] = myround(tmp_rep_tr.data[2][2] * one);
	deletefloat2D(&tmp_rep_tr);

	srcdata = src->data;
	dstdata = dst->data;
#if LINEAR_INTERPOLATION
	for(i = i_min_y; i < i_max_y; i++)
	{
		for(j = i_min_x; j < i_max_x; j++)
		{
			pseudo_float x = rep_tr.data[0][0] * j + rep_tr.data[0][1] * i + rep_tr.data[0][2];
			pseudo_float y = rep_tr.data[1][0] * j + rep_tr.data[1][1] * i + rep_tr.data[1][2];
			if(x >= 0 && x < p_width && y >= 0 && y < p_height)
			{
				int up_y = pseudo_ceil(y);
				int bo_y = pseudo_floor(y);
				int up_x = pseudo_ceil(x);
				int bo_x = pseudo_floor(x);

				pseudo_float f_up_y = up_y * one;
				pseudo_float f_bo_y = bo_y * one;
				pseudo_float f_up_x = up_x * one;
				pseudo_float f_bo_x = bo_x * one;

				pseudo_float out = (pseudo_float) srcdata[bo_y][bo_x] * (f_up_y - y) * (f_up_x - x) +
					(pseudo_float) srcdata[bo_y][up_x] * (f_up_y - y) * (x - f_bo_x) +
					(pseudo_float) srcdata[up_y][bo_x] * (y - f_bo_y) * (f_up_x - x) +
					(pseudo_float) srcdata[up_y][up_x] * (y - f_bo_y) * (x - f_bo_x);
#if FLOAT_ROUND
				out = out + a_half2;
				out /= one2;
#else
				out /= one2;
#endif
				dstdata[i][j] = (unsigned char)out;
			}
		}
	}
#else
	for(i = i_min_y; i < i_max_y; i++)
	{
		for(j = i_min_x; j < i_max_x; j++)
		{
			int m = pseudo_round(rep_tr.data[0][0] * i_min_x + rep_tr.data[0][1] * i + rep_tr.data[0][2]);
			int n = pseudo_round(rep_tr.data[1][0] * i_min_x + rep_tr.data[1][1] * i + rep_tr.data[1][2]);
			if(m >= 0 && m < width && n >= 0 && n < height)
			{
				dstdata[i][j] = srcdata[n][m];
			}
		}
	}
#endif
	deletepseudo_float2D(&rep_tr);

	return out;
}

int imgWarpAffineColor(BYTE2Dcolor *src, BYTE2Dcolor *dst, Float2Dgray *tr)	//2.3.0 warp fix
{
	int width = src->width;
	int height = src->height;
	pseudo_float p_width = (width - 1) * one;
	pseudo_float p_height = (height - 1) * one;
	int i, j, out;

	float max_x = 0.0f, min_x = (float)dst->width, max_y = 0.0f, min_y = (float)dst->height;
	int i_max_x, i_min_x, i_max_y, i_min_y;
	Point2D32f tmp_p, tmp_p1, tmp_p2;
	float m, b;
	Float2Dgray tmp_rep_tr;
	Pseudo_float2Dgray rep_tr;
	RgbPixel **srcdata, **dstdata;
	float **trdata = tr->data;

	if(tr->width != 3 || tr->height != 3)
	{
		return width - 1;
	}

	//make2Dfloatarray(&tmp, 1, 3);
	//compute size of warping place
	i = 0;
	j = 0;
	tmp_p.x = trdata[0][0] * i + trdata[0][1] * j + trdata[0][2];
	tmp_p.y = trdata[1][0] * i + trdata[1][1] * j + trdata[1][2];
	if(max_x < tmp_p.x)
	{
		max_x = tmp_p.x;
	}

	if(min_x > tmp_p.x)
	{
		min_x = tmp_p.x;
	}

	if(max_y < tmp_p.y)
	{
		max_y = tmp_p.y;
	}

	if(min_y > tmp_p.y)
	{
		min_y = tmp_p.y;
	}

	tmp_p1 = tmp_p;

	i = width - 1;
	tmp_p.x = trdata[0][0] * i + trdata[0][1] * j + trdata[0][2];
	tmp_p.y = trdata[1][0] * i + trdata[1][1] * j + trdata[1][2];
	if(max_x < tmp_p.x)
	{
		max_x = tmp_p.x;
	}

	if(min_x > tmp_p.x)
	{
		min_x = tmp_p.x;
	}

	if(max_y < tmp_p.y)
	{
		max_y = tmp_p.y;
	}

	if(min_y > tmp_p.y)
	{
		min_y = tmp_p.y;
	}

	i = 0;
	j = height - 1;
	tmp_p.x = trdata[0][0] * i + trdata[0][1] * j + trdata[0][2];
	tmp_p.y = trdata[1][0] * i + trdata[1][1] * j + trdata[1][2];
	if(max_x < tmp_p.x)
	{
		max_x = tmp_p.x;
	}

	if(min_x > tmp_p.x)
	{
		min_x = tmp_p.x;
	}

	if(max_y < tmp_p.y)
	{
		max_y = tmp_p.y;
	}

	if(min_y > tmp_p.y)
	{
		min_y = tmp_p.y;
	}

	tmp_p2 = tmp_p;

	i = width - 1;
	tmp_p.x = trdata[0][0] * i + trdata[0][1] * j + trdata[0][2];
	tmp_p.y = trdata[1][0] * i + trdata[1][1] * j + trdata[1][2];
	if(max_x < tmp_p.x)
	{
		max_x = tmp_p.x;
	}

	if(min_x > tmp_p.x)
	{
		min_x = tmp_p.x;
	}

	if(max_y < tmp_p.y)
	{
		max_y = tmp_p.y;
	}

	if(min_y > tmp_p.y)
	{
		min_y = tmp_p.y;
	}

	if(tmp_p1.y < 0 || tmp_p2.y < 0 || tmp_p1.y > height - 1 || tmp_p2.y > height - 1)
	{
		m = (tmp_p1.x - tmp_p2.x) / (tmp_p1.y - tmp_p2.y);
		b = tmp_p1.x - m * tmp_p1.y;

		out = ceil(m * (float)(height - 1) + b);
		out = max(ceil(b), out);
	}
	else
	{
		out = ceil(max(tmp_p1.x, tmp_p2.x));
	}

	i_min_x = max(floor(min_x), 0);

	if(i_min_x < trdata[0][2])
	{
		i_min_x = (int)trdata[0][2];	//2.2.0 bug fix
	}

	i_min_y = max(floor(min_y), 0);
	i_max_x = min(ceil(max_x), dst->width);
	i_max_y = min(ceil(max_y), dst->height);

	//Rect.x = min_x;
	//Rect.y = min_y;
	//Rect.width = max_x - min_x + 1;
	//Rect.height = max_y - min_y + 1;
	if(FALSE == make2Dfloatarray(&tmp_rep_tr, 3, 3))
	{
		return MEMORY_FAIL;
	}

	tmp_rep_tr.data[0][0] = trdata[0][0];
	tmp_rep_tr.data[0][1] = trdata[0][1];
	tmp_rep_tr.data[0][2] = trdata[0][2];
	tmp_rep_tr.data[1][0] = trdata[1][0];
	tmp_rep_tr.data[1][1] = trdata[1][1];
	tmp_rep_tr.data[1][2] = trdata[1][2];
	tmp_rep_tr.data[2][0] = trdata[2][0];
	tmp_rep_tr.data[2][1] = trdata[2][1];
	tmp_rep_tr.data[2][2] = trdata[2][2];
	invert(&tmp_rep_tr);

	if(FALSE == make2Dpseudo_floatarray(&rep_tr, 3, 3))
	{
		return MEMORY_FAIL;
	}

	rep_tr.data[0][0] = myround(tmp_rep_tr.data[0][0] * one);
	rep_tr.data[0][1] = myround(tmp_rep_tr.data[0][1] * one);
	rep_tr.data[0][2] = myround(tmp_rep_tr.data[0][2] * one);
	rep_tr.data[1][0] = myround(tmp_rep_tr.data[1][0] * one);
	rep_tr.data[1][1] = myround(tmp_rep_tr.data[1][1] * one);
	rep_tr.data[1][2] = myround(tmp_rep_tr.data[1][2] * one);
	rep_tr.data[2][0] = myround(tmp_rep_tr.data[2][0] * one);
	rep_tr.data[2][1] = myround(tmp_rep_tr.data[2][1] * one);
	rep_tr.data[2][2] = myround(tmp_rep_tr.data[2][2] * one);
	deletefloat2D(&tmp_rep_tr);

	srcdata = src->data;
	dstdata = dst->data;
#if LINEAR_INTERPOLATION
	for(i = i_min_y; i < i_max_y; i++)
	{
		for(j = i_min_x; j < i_max_x; j++)
		{
			pseudo_float x = rep_tr.data[0][0] * j + rep_tr.data[0][1] * i + rep_tr.data[0][2];
			pseudo_float y = rep_tr.data[1][0] * j + rep_tr.data[1][1] * i + rep_tr.data[1][2];
			if(x >= 0 && x < p_width && y >= 0 && y < p_height)
			{
				int up_y = pseudo_ceil(y);
				int bo_y = pseudo_floor(y);
				int up_x = pseudo_ceil(x);
				int bo_x = pseudo_floor(x);

				pseudo_float f_up_y = up_y * one;
				pseudo_float f_bo_y = bo_y * one;
				pseudo_float f_up_x = up_x * one;
				pseudo_float f_bo_x = bo_x * one;

				// R
				pseudo_float out = (pseudo_float)srcdata[bo_y][bo_x].r * (f_up_y - y) * (f_up_x - x) +
					(pseudo_float)srcdata[bo_y][up_x].r * (f_up_y - y) * (x - f_bo_x) +
					(pseudo_float)srcdata[up_y][bo_x].r * (y - f_bo_y) * (f_up_x - x) +
					(pseudo_float)srcdata[up_y][up_x].r * (y - f_bo_y) * (x - f_bo_x);
#if FLOAT_ROUND
				out = out + a_half2;
				out /= one2;
#else
				out /= one2;
#endif
				dstdata[i][j].r = (unsigned char)out;

				// G
				out = (pseudo_float)srcdata[bo_y][bo_x].g * (f_up_y - y) * (f_up_x - x) +
					(pseudo_float)srcdata[bo_y][up_x].g * (f_up_y - y) * (x - f_bo_x) +
					(pseudo_float)srcdata[up_y][bo_x].g * (y - f_bo_y) * (f_up_x - x) +
					(pseudo_float)srcdata[up_y][up_x].g * (y - f_bo_y) * (x - f_bo_x);
#if FLOAT_ROUND
				out = out + a_half2;
				out /= one2;
#else
				out /= one2;
#endif
				dstdata[i][j].g = (unsigned char)out;

				// B
				out = (pseudo_float)srcdata[bo_y][bo_x].b * (f_up_y - y) * (f_up_x - x) +
					(pseudo_float)srcdata[bo_y][up_x].b * (f_up_y - y) * (x - f_bo_x) +
					(pseudo_float)srcdata[up_y][bo_x].b * (y - f_bo_y) * (f_up_x - x) +
					(pseudo_float)srcdata[up_y][up_x].b * (y - f_bo_y) * (x - f_bo_x);
#if FLOAT_ROUND
				out = out + a_half2;
				out /= one2;
#else
				out /= one2;
#endif
				dstdata[i][j].b = (unsigned char)out;
			}
		}
	}
#else
	for(i = i_min_y; i < i_max_y; i++)
	{
		for(j = i_min_x; j < i_max_x; j++)
		{
			int m = pseudo_round(rep_tr.data[0][0] * i_min_x + rep_tr.data[0][1] * i + rep_tr.data[0][2]);
			int n = pseudo_round(rep_tr.data[1][0] * i_min_x + rep_tr.data[1][1] * i + rep_tr.data[1][2]);
			if(m >= 0 && m < width && n >= 0 && n < height)
			{
				dstdata[i][j].r = srcdata[n][m].r;
				dstdata[i][j].g = srcdata[n][m].g;
				dstdata[i][j].b = srcdata[n][m].b;
			}
		}
	}
#endif
	deletepseudo_float2D(&rep_tr);

	return out;
}

#if 0
void makeIntergalimg(BYTE2Dgray* input, Int2Dgray* output)
{
	int i, j;
	int width = input->width, height = input->height;
	unsigned char **inputdata = input->data;
	int **outputdata = output->data;

	outputdata[0][0] = (int)inputdata[0][0];
	for(j = 1; j < width; j++)
	{
		output->data[0][j] = outputdata[0][j - 1] + (int)inputdata[0][j];
	}
	for(i = 1; i < height; i++)
	{
		outputdata[i][0] = outputdata[i - 1][0] + (int)inputdata[i][0];
	}
	for(i = 1; i < height; i++)
	{
		for(j = 1; j < width; j++)
		{
			outputdata[i][j] = outputdata[i][j - 1] + outputdata[i - 1][j] - outputdata[i - 1][j - 1] + (int)inputdata[i][j];
		}
	}	
}
#endif

// 2011-01-07 scan code copy
int makeIntergalimg(BYTE2Dgray *input, Int2Dgray *output)
{
	int i, j;
	int width = input->width, height = input->height;
	int *rowSum = NULL, *pRowSum = NULL, *pSum = NULL, *pSumOld = NULL;
	int *pOut = NULL, *pOutOld = NULL, *pOutTmp = NULL;
	unsigned char *pIn = NULL, *pInOld = NULL;

	rowSum = (int *)calloc(height, sizeof(int));
	if(rowSum == NULL)	//add.jw.2011-01-07
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
		return MEMORY_FAIL;
	}

	pOut = output->data[0], pIn = input->data[0], *pOut = *pIn;
	for(i = 1; i < height; i++)
	{
		pOutOld = pOut, pOut += width, pIn += width;
		*pOut = *pOutOld +*pIn;
	}

	pRowSum = rowSum, pIn = input->data[0], pOut = output->data[0];
	for(j = 1; j < width; j++)
	{
		pOutOld = pOut++, pOutTmp = pOut, pIn++, pInOld = pIn, pSum = pRowSum;
		*pRowSum = *pIn;
		*pOut = *pOutOld +*pRowSum;
		for(i = 1; i < height; i++)
		{
			pOutTmp += width, pOutOld += width, pSumOld = pSum++, pInOld += width;
			*pSum = *pSumOld +*pInOld;
			*pOutTmp = *pOutOld +*pSum;
		}
	}

	free(rowSum);
	return FALSE;
}

int blobdetector(BYTE2Dgray *input, Point2D32f *feature, int *max_points, int threshold_ratio)
{
	int i, j, count = *max_points;
	int width = input->width;
	int height = input->height;
	int tmp_w, tmp_h, tmp1, tmp2, tmp3, tmp4;
	int threshold, max_det = 0;

	Int2Dgray int_img, det;

	if(FALSE == make2Dintarray(&int_img, width, height))
	{
		return MEMORY_FAIL;
	}

	if(MEMORY_FAIL == makeIntergalimg(input, &int_img))
	{
		return MEMORY_FAIL;
	}

	deleteBYTE2D(input);
	if(FALSE == make2Dintarray(&det, width, height))
	{
		return MEMORY_FAIL;
	}

	memset(det.data[0], 0, width * height * sizeof(int));

	tmp_h = height - 5;
	tmp_w = width - 5;

	for(i = 5; i < tmp_h; i++)
	{
		for(j = 5; j < tmp_w; j++)
		{
			int dxx, dxy, dyy;
			dxx = int_img.data[i + 2][j + 4] - int_img.data[i - 3][j + 4] - int_img.data[i + 2][j - 5] + int_img.data[i - 3][j - 5];
			dxx -= (int_img.data[i + 2][j + 1] - int_img.data[i - 3][j + 1] - int_img.data[i + 2][j - 2] + int_img.data[i - 3][j - 2]) * 3;

			dyy = int_img.data[i + 4][j + 2] - int_img.data[i - 5][j + 2] - int_img.data[i + 4][j - 3] + int_img.data[i - 5][j - 3];
			dyy -= (int_img.data[i + 1][j + 2] - int_img.data[i - 2][j + 2] - int_img.data[i + 1][j - 3] + int_img.data[i - 2][j - 3]) * 3;

			dxy = int_img.data[i - 1][j - 1] - int_img.data[i - 4][j - 1] - int_img.data[i - 1][j - 4] + int_img.data[i - 4][j - 4];
			dxy -= int_img.data[i - 1][j + 3] - int_img.data[i - 4][j + 3] - int_img.data[i - 1][j] + int_img.data[i - 4][j];
			dxy -= int_img.data[i + 3][j - 1] - int_img.data[i][j - 1] - int_img.data[i + 3][j - 4] + int_img.data[i][j - 4];
			dxy += int_img.data[i + 3][j + 3] - int_img.data[i][j + 3] - int_img.data[i + 3][j] + int_img.data[i][j];

			det.data[i][j] = (dxx * dyy - dxy * dxy * 81 / 100);
			if(max_det < det.data[i][j])
			{
				max_det = det.data[i][j];
			}
		}
	}

	deleteint2D(&int_img);

	if(max_det == 0)
	{
		deleteint2D(&det);
		*max_points = 0;
		return FALSE;
	}

	threshold = max_det / threshold_ratio;

	while(count == *max_points)
	{
		count = 0;
		for(i = 5; i < tmp_h; i++)
		{
			for(j = 5; j < tmp_w; j++)
			{
				if(det.data[i][j] < threshold)
				{
					continue;
				}

				if(det.data[i][j] < det.data[(i - 1)][j - 1])
				{
					continue;
				}

				if(det.data[i][j] < det.data[(i - 1)][j])
				{
					continue;
				}

				if(det.data[i][j] < det.data[(i - 1)][j + 1])
				{
					continue;
				}

				if(det.data[i][j] < det.data[i][j - 1])
				{
					continue;
				}

				if(det.data[i][j] < det.data[i][j + 1])
				{
					continue;
				}

				if(det.data[i][j] < det.data[(i + 1)][j - 1])
				{
					continue;
				}

				if(det.data[i][j] < det.data[(i + 1)][j])
				{
					continue;
				}

				if(det.data[i][j] < det.data[(i + 1)][j + 1])
				{
					continue;
				}

				if(count >= *max_points)
				{
					break;
				}

#if SUB_PIXEL_ACCURACY
				tmp1 = det.data[i][j + 1] - 2 * det.data[i][j] + det.data[i][j - 1];
				tmp2 = det.data[i + 1][j] - 2 * det.data[i][j] + det.data[i - 1][j];
				tmp3 = (det.data[i][j + 1] - det.data[i][j - 1]) / 2.0f;
				tmp4 = (det.data[i + 1][j] - det.data[i - 1][j]) / 2.0f;

				feature[count].x = (float)j - tmp3 / tmp1;
				feature[count].y = (float)i - tmp4 / tmp2;
#else
				feature[count].x = (float)j;
				feature[count].y = (float)i;
#endif
				count++;
				j++;
			}
		}

		threshold = threshold * 3 / 2;
	}

	deleteint2D(&det);
	*max_points = count;

	return FALSE;
}

int harrisCorner(Float2Dgray *input, Point2D32f *feature, int *max_points, int threshold_ratio)
{
	int i, j, count;
	float max_eig = 0.0f;
	float threshold;
	float tmp1, tmp2, tmp3, tmp4;
	//float tmp5;
	
	int width = input->width;
	int height = input->height;

	//BmpImage out_img;
	Float2Dgray g_in, d_g_x, d_g_y, d_g_xy, g_d_xx, g_d_xy, g_d_yy, f_HM;
	if(FALSE == make2Dfloatarray(&g_in, width, height))
	{
		return MEMORY_FAIL;
	}

	gaussian(input, &g_in, 1.0);

	deletefloat2D(input);

	//floatarray2Bmp(&g_in, &orgimgL, &out_img);
	//writeBmp("gaussian_result.bmp", &out_img, FALSE);
	//deleteBmp(&out_img);
	if(FALSE == make2Dfloatarray(&d_g_x, width, height))
	{
		return MEMORY_FAIL;
	}

	if(FALSE == make2Dfloatarray(&d_g_y, width, height))
	{
		return MEMORY_FAIL;
	}

	memset(d_g_x.data[0], 0, width * height * sizeof(float));
	memset(d_g_y.data[0], 0, width * height * sizeof(float));

	for(i = 1; i < height - 1; i++)
	{
		for(j = 1; j < width - 1; j++)
		{
			d_g_x.data[i][j] = (g_in.data[i][j + 1] - g_in.data[i][j - 1]) / 2.0f;
			d_g_y.data[i][j] = (g_in.data[(i + 1)][j] - g_in.data[(i - 1)][j]) / 2.0f;
		}
	}

	deletefloat2D(&g_in);

	//Compute Harris Matrix
	if(FALSE == make2Dfloatarray(&d_g_xy, width, height))
	{
		return MEMORY_FAIL;
	}

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			d_g_xy.data[i][j] = d_g_x.data[i][j] * d_g_y.data[i][j];
			d_g_x.data[i][j] = d_g_x.data[i][j] * d_g_x.data[i][j];
			d_g_y.data[i][j] = d_g_y.data[i][j] * d_g_y.data[i][j];
		}
	}

	//Harris Matrix -> Gaussian Blur with sigma=1.5
	//DBG_PRINTF_SDH_DEBUG("Harris Matrix -> Gaussian Blur with sigma = 1.5\n");
	if(FALSE == make2Dfloatarray(&g_d_xy, width, height))
	{
		return MEMORY_FAIL;
	}

	gaussian(&d_g_xy, &g_d_xy, 1.5);
	deletefloat2D(&d_g_xy);
	if(FALSE == make2Dfloatarray(&g_d_xx, width, height))
	{
		return MEMORY_FAIL;
	}

	gaussian(&d_g_x, &g_d_xx, 1.5);
	deletefloat2D(&d_g_x);
	if(FALSE == make2Dfloatarray(&g_d_yy, width, height))
	{
		return MEMORY_FAIL;
	}

	gaussian(&d_g_y, &g_d_yy, 1.5);
	deletefloat2D(&d_g_y);

	//floatarray2Bmp(&g_d_yy, &orgimgL, &out_img);
	//writeBmp("gaussian_result.bmp", &out_img, FALSE);
	//deleteBmp(&out_img);
	
	//Compute corner detection function
	//DBG_PRINTF_SDH_DEBUG("Compute corner detection function\n");
	if(FALSE == make2Dfloatarray(&f_HM, width, height))
	{
		return MEMORY_FAIL;
	}

#if FAST_CONV
	memset(f_HM.data[0], 0, width * height * sizeof(float));
	for(i = 7; i < height - 7; i++)
	{
		for(j = 7; j < width - 7; j++)
		{
			tmp1 = g_d_xx.data[i][j];
			tmp2 = g_d_xy.data[i][j];
			tmp3 = g_d_yy.data[i][j];

			f_HM.data[i][j] = (tmp1 + tmp3 - (float)sqrt(tmp1 * tmp1 + 4.0f * tmp2 * tmp2 - 2.0f * tmp1 * tmp3 + tmp3 * tmp3));
			if(f_HM.data[i][j] > max_eig)
			{
				max_eig = f_HM.data[i][j];
			}
		}
	}
#else
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			tmp1 = g_d_xx.data[i][j];
			tmp2 = g_d_xy.data[i][j];
			tmp3 = g_d_yy.data[i][j];

			f_HM.data[i][j] = (tmp1 + tmp3 - sqrt(tmp1 * tmp1 + 4.0f * tmp2 * tmp2 - 2.0f * tmp1 * tmp3 + tmp3 * tmp3));
			if(f_HM.data[i][j] > max_eig)
			{
				max_eig = f_HM.data[i][j];
			}
		}
	}
#endif
	deletefloat2D(&g_d_xx);
	deletefloat2D(&g_d_xy);
	deletefloat2D(&g_d_yy);

	threshold = max_eig / (float)threshold_ratio;

	//Find local maximum
	//DBG_PRINTF_SDH_DEBUG("Find local maximum\n");
	count = 0;
#if SUB_PIXEL_ACCURACY
	for(i = 1; i < height - 1; i++)
	{
		for(j = 1; j < width - 1; j++)
		{
			if(f_HM.data[i][j] < threshold)
			{
				continue;
			}

			if(f_HM.data[i][j] < f_HM.data[(i - 1)][j - 1])
			{
				continue;
			}

			if(f_HM.data[i][j] < f_HM.data[(i - 1)][j])
			{
				continue;
			}

			if(f_HM.data[i][j] < f_HM.data[(i - 1)][j + 1])
			{
				continue;
			}

			if(f_HM.data[i][j] < f_HM.data[i][j - 1])
			{
				continue;
			}

			if(f_HM.data[i][j] < f_HM.data[i][j + 1])
			{
				continue;
			}

			if(f_HM.data[i][j] < f_HM.data[(i + 1)][j - 1])
			{
				continue;
			}

			if(f_HM.data[i][j] < f_HM.data[(i + 1)][j])
			{
				continue;
			}

			if(f_HM.data[i][j] < f_HM.data[(i + 1)][j + 1])
			{
				continue;
			}

			if(count >= *max_points)
			{
				break;
			}

			tmp1 = f_HM.data[i][j + 1] - 2 * f_HM.data[i][j] + f_HM.data[i][j - 1];
			tmp2 = f_HM.data[i + 1][j] - 2 * f_HM.data[i][j] + f_HM.data[i - 1][j];
			tmp3 = (f_HM.data[i][j + 1] - f_HM.data[i][j - 1]) / 2.0f;
			tmp4 = (f_HM.data[i + 1][j] - f_HM.data[i - 1][j]) / 2.0f;

			feature[count].x = (float)j - tmp3 / tmp1;
			feature[count].y = (float)i - tmp4 / tmp2;
			count++;
			j++;
		}
	}
#else
	for(i = 1; i < height - 1; i++)
	{
		for(j = 1; j < width - 1; j++)
		{
			if(f_HM.data[i][j] < threshold)
			{
				continue;
			}

			if(f_HM.data[i][j] < f_HM.data[(i - 1)][j - 1])
			{
				continue;
			}

			if(f_HM.data[i][j] < f_HM.data[(i - 1)][j])
			{
				continue;
			}

			if(f_HM.data[i][j] < f_HM.data[(i - 1)][j + 1])
			{
				continue;
			}

			if(f_HM.data[i][j] < f_HM.data[i][j - 1])
			{
				continue;
			}

			if(f_HM.data[i][j] < f_HM.data[i][j + 1])
			{
				continue;
			}

			if(f_HM.data[i][j] < f_HM.data[(i + 1)][j - 1])
			{
				continue;
			}

			if(f_HM.data[i][j] < f_HM.data[(i + 1)][j])
			{
				continue;
			}

			if(f_HM.data[i][j] < f_HM.data[(i + 1)][j + 1])
			{
				continue;
			}

			if(count >= *max_points)
			{
				break;
			}

			feature[count].x = (float)j;
			feature[count].y = (float)i;
			count++;
			j++;
		}
	}
#endif

	//DBG_PRINTF_SDH_DEBUG("max sub pixel = %lf\n", tmp5);
	deletefloat2D(&f_HM);
	*max_points = count;

	return FALSE;
}

int *computePutativeCorrespondence(int srcpt_size, int dstpt_size, Point2D32f *src_pt, Point2D32f *dst_pt,
								   BYTE2Dgray *src, BYTE2Dgray *dst, int overlap_width, int *real_putative1,
								   int *real_putative_num)
{
	const int DISTANCE_THRESHOLD = MAX_DIFF;
	const int BLOCK_RADIUS = 3;

	int i, j;
	int r_num = 0;

	int nWidth = dst->width;

	//int nHeight = dst->height;
	int width_offset = nWidth - overlap_width;

	unsigned char **srcdata = src->data, **dstdata = dst->data;

	int *putative_correspondence = (int *)calloc(srcpt_size, sizeof(int));
	if(NULL == putative_correspondence) //add.bong.2010-11-25 10:26AM
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, srcpt_size);	//add.bong.2010-11-25 10:26AM
		return putative_correspondence;
	}

	for(i = 0; i < srcpt_size; ++i)
	{
		int min_index = 0;

		int srcx = round(src_pt[i].x);
		int srcy = round(src_pt[i].y);

		int min_distance = 2000000000;
		int min_2nd_distance = 2000000000;

		for(j = 0; j < dstpt_size; ++j)
		{
			int dstx = round(dst_pt[j].x);
			int dsty = round(dst_pt[j].y);

			if(myabs(srcy - dsty) <= DISTANCE_THRESHOLD)
			{
				int row, col;
				int distance = 0;

				for(col = -BLOCK_RADIUS; col <= BLOCK_RADIUS; col++)
				{
					for(row = -BLOCK_RADIUS; row <= BLOCK_RADIUS; row++)
					{
						int tmp = srcdata[(srcy + col)][srcx + row + width_offset] - dstdata[(dsty + col)][dstx + row];
						distance += tmp * tmp;
					}
				}

				if(distance <= min_distance)
				{
					min_2nd_distance = min_distance;
					min_distance = distance;
					min_index = j;
				}
				else if(distance > min_distance && distance < min_2nd_distance)
				{
					min_2nd_distance = distance;
				}
			}
		}

		if((double)min_distance / min_2nd_distance < 0.64)
		{
			putative_correspondence[i] = min_index;
			real_putative1[r_num] = i;
			r_num++;
		}
		else
		{
			putative_correspondence[i] = -1;
		}
	}

	if(r_num == 0)	// add.ljh.2011-01-14
	{
		if(srcpt_size > dstpt_size)
		{
			r_num = dstpt_size;
		}
		else
		{
			r_num = srcpt_size;
		}
	}

	*real_putative_num = r_num;

	return putative_correspondence;
}

int *computePutativeCorrespondenceColor(int srcpt_size, int dstpt_size, Point2D32f *src_pt, Point2D32f *dst_pt,
								   BYTE2Dcolor *src, BYTE2Dcolor *dst, int overlap_width, int *real_putative1,
								   int *real_putative_num)
{
	const int DISTANCE_THRESHOLD = MAX_DIFF;
	const int BLOCK_RADIUS = 3;

	int i, j;
	int r_num = 0;

	int nWidth = dst->width;

	//int nHeight = dst->height;
	int width_offset = nWidth - overlap_width;

	RgbPixel **srcdata = src->data, **dstdata = dst->data;

	int *putative_correspondence = (int *)calloc(srcpt_size, sizeof(int));
	if(NULL == putative_correspondence) //add.bong.2010-11-25 10:26AM
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, srcpt_size);	//add.bong.2010-11-25 10:26AM
		return putative_correspondence;
	}

	for(i = 0; i < srcpt_size; ++i)
	{
		int min_index = 0;

		int srcx = round(src_pt[i].x);
		int srcy = round(src_pt[i].y);

		int min_distance = 2000000000;
		int min_2nd_distance = 2000000000;

		for(j = 0; j < dstpt_size; ++j)
		{
			int dstx = round(dst_pt[j].x);
			int dsty = round(dst_pt[j].y);

			if(myabs(srcy - dsty) <= DISTANCE_THRESHOLD)
			{
				int row, col;
				int distance = 0;

				for(col = -BLOCK_RADIUS; col <= BLOCK_RADIUS; col++)
				{
					for(row = -BLOCK_RADIUS; row <= BLOCK_RADIUS; row++)
					{
						int tmp = ((srcdata[(srcy + col)][srcx + row + width_offset].r +
							srcdata[(srcy + col)][srcx + row + width_offset].g +
							srcdata[(srcy + col)][srcx + row + width_offset].b) -
							(dstdata[(dsty + col)][dstx + row].r +
							dstdata[(dsty + col)][dstx + row].g +
							dstdata[(dsty + col)][dstx + row].b)) / 3;
						distance += tmp * tmp;
					}
				}

				if(distance <= min_distance)
				{
					min_2nd_distance = min_distance;
					min_distance = distance;
					min_index = j;
				}
				else if(distance > min_distance && distance < min_2nd_distance)
				{
					min_2nd_distance = distance;
				}
			}
		}

		if((double)min_distance / min_2nd_distance < 0.64)
		{
			putative_correspondence[i] = min_index;
			real_putative1[r_num] = i;
			r_num++;
		}
		else
		{
			putative_correspondence[i] = -1;
		}
	}

	if(r_num == 0)	// add.ljh.2011-01-14
	{
		if(srcpt_size > dstpt_size)
		{
			r_num = dstpt_size;
		}
		else
		{
			r_num = srcpt_size;
		}
	}

	*real_putative_num = r_num;

	return putative_correspondence;
}

int locateFeaturePoints(BYTE2Dgray *scanimage, int nowidth, int noheight, Point2D32f *featurePoints, int downsampling,
						int overlap_width, BOOL isLeft)
{
	int i, j;

	BYTE2Dgray cropimg;
	int nPoints = MAX_FEATURE / downsampling;

	unsigned char **scanimagedata = scanimage->data;

	if(FALSE == make2DBYTEarray(&cropimg, overlap_width, noheight))
	{
		return MEMORY_FAIL;
	}

	if(isLeft)
	{
		for(i = 0; i < noheight; i++)
		{
			for(j = 0; j < overlap_width; j++)
			{
				cropimg.data[i][j] = scanimagedata[i][j + nowidth - overlap_width];
			}
		}
	}
	else
	{
		for(i = 0; i < noheight; i++)
		{
			for(j = 0; j < overlap_width; j++)
			{
				cropimg.data[i][j] = scanimagedata[i][j];
			}
		}
	}

	if(downsampling != 1)
	{
		if(MEMORY_FAIL == resize(&cropimg, downsampling))
		{
			return MEMORY_FAIL;
		}
	}

	//featurePoints = (Point2D32f *)calloc(nPoints, sizeof(Point2D32f));
	//harrisCorner(&imgL, featurePointsL, &nPointsL, CORNER_STRENGTH);
	if(MEMORY_FAIL == blobdetector(&cropimg, featurePoints, &nPoints, CORNER_STRENGTH))
	{
		return MEMORY_FAIL;
	}

	if(downsampling != 1)
	{
#if RESIZE_QUALITY
		for(i = 0; i < nPoints; i++)
		{
			featurePoints[i].x = (featurePoints[i].x + 0.5f) * downsampling - 0.5f;
			featurePoints[i].y = (featurePoints[i].y + 0.5f) * downsampling - 0.5f;
		}
#else
		for(i = 0; i < nPoints; i++)
		{
			featurePoints[i].x = (featurePoints[i].x) * downsampling;
			featurePoints[i].y = (featurePoints[i].y) * downsampling;
		}
#endif
	}

	return nPoints;
}


int locateFeaturePointsColor(BYTE2Dcolor *scanimage, int nowidth, int noheight, Point2D32f *featurePoints, int downsampling,
						int overlap_width, BOOL isLeft)
{
	int i, j;

	BYTE2Dgray cropimg;
	int nPoints = MAX_FEATURE / downsampling;

	RgbPixel **scanimagedata = scanimage->data;

	if(FALSE == make2DBYTEarray(&cropimg, overlap_width, noheight))
	{
		return MEMORY_FAIL;
	}

	if(isLeft)
	{
		for(i = 0; i < noheight; i++)
		{
			for(j = 0; j < overlap_width; j++)
			{
				cropimg.data[i][j] = (scanimagedata[i][j + nowidth - overlap_width].r +
					scanimagedata[i][j + nowidth - overlap_width].g +
					scanimagedata[i][j + nowidth - overlap_width].b) / 3;
			}
		}
	}
	else
	{
		for(i = 0; i < noheight; i++)
		{
			for(j = 0; j < overlap_width; j++)
			{
				cropimg.data[i][j] = (scanimagedata[i][j].r + scanimagedata[i][j].g + scanimagedata[i][j].b) / 3;
			}
		}
	}

	if(downsampling != 1)
	{
		if(MEMORY_FAIL == resize(&cropimg, downsampling))
		{
			return MEMORY_FAIL;
		}
	}

	//featurePoints = (Point2D32f *)calloc(nPoints, sizeof(Point2D32f));
	//harrisCorner(&imgL, featurePointsL, &nPointsL, CORNER_STRENGTH);
	if(MEMORY_FAIL == blobdetector(&cropimg, featurePoints, &nPoints, CORNER_STRENGTH))
	{
		return MEMORY_FAIL;
	}

	if(downsampling != 1)
	{
#if RESIZE_QUALITY
		for(i = 0; i < nPoints; i++)
		{
			featurePoints[i].x = (featurePoints[i].x + 0.5f) * downsampling - 0.5f;
			featurePoints[i].y = (featurePoints[i].y + 0.5f) * downsampling - 0.5f;
		}
#else
		for(i = 0; i < nPoints; i++)
		{
			featurePoints[i].x = (featurePoints[i].x) * downsampling;
			featurePoints[i].y = (featurePoints[i].y) * downsampling;
		}
#endif
	}

	return nPoints;
}

int findmotionvector(unsigned char **limg, unsigned char **rimg, int nowidth, int noheight, int overlap_width, float *out_tx, float *out_ty)
{
	DBG_PRINTF_SDH_DEBUG("findmotionvector entry\n");

	int i, j;
	int m, n;

	//add.jw.2011-01-28: snu modified search region
	int windowsize_w = overlap_width / 6;
	int windowsize_h = a3_dlt_height / 8;
	int center_w = overlap_width - windowsize_w;
	int center_h = a3_dlt_height / 2;
	int thre_h = 8;
	int thre_w = overlap_width / 6;

	int new_center_h = 0;
	int new_center_w = 0;

	float tmp = 0;
	float sad_val = 0;

	int ind_i = 0;
	int ind_j = 0;

	float min_sad = 20000000;
	float min_sad2 = 20000000;

	int center_ww = windowsize_w;

	// Mask
	BYTE2Dgray imgL;
	if(FALSE == make2DBYTEarray(&imgL, overlap_width, noheight))
	{
		return MEMORY_FAIL;
	}

	for(i = 0; i < noheight; i++)
	{
		for(j = 0; j < overlap_width; j++)
		{
			imgL.data[i][j] = limg[i][j + nowidth - overlap_width];
		}
	}

	for(i = -thre_h; i < thre_h + 1; i++)
	{
		new_center_h = center_h + i;

		for(j = 0; j < thre_w + 1; j++)
		{
			new_center_w = center_ww + j;

			sad_val = 0;
			for(m = -windowsize_h; m < windowsize_h + 1; m++)
			{
				for(n = -windowsize_w; n < windowsize_w + 1; n++)
				{
					tmp = (float)(imgL.data[center_h + m][center_w + n]) - (float)(rimg[new_center_h + m][new_center_w + n]);
					tmp = abs(tmp);
					sad_val += tmp;
				}
			}

			if(sad_val < min_sad)
			{
				min_sad = sad_val;
				ind_i = i;
				ind_j = j;
			}
			else if((sad_val > min_sad) && (sad_val < min_sad2))
			{
				min_sad2 = sad_val;
			}
		}
	}

	if(min_sad / min_sad2 * 100.0 > 99.5)
	{
		ind_i = 0;
		ind_j = 0;
	}

	deleteBYTE2D(&imgL);

	*out_tx = (float)overlap_width - (2 * windowsize_w + 1) - (ind_j);
	*out_ty = (float)(-ind_i);

	return FALSE;
}

int findmotionvectorColor(RgbPixel **limg, RgbPixel **rimg, int nowidth, int noheight, int overlap_width, float *out_tx, float *out_ty)
{
	DBG_PRINTF_SDH_DEBUG("findmotionvector entry\n");

	int i, j;
	int m, n;

	//add.jw.2011-01-28: snu modified search region
	int windowsize_w = overlap_width / 6;
	int windowsize_h = a3_dlt_height / 8;
	int center_w = overlap_width - windowsize_w;
	int center_h = a3_dlt_height / 2;
	int thre_h = 8;
	int thre_w = overlap_width / 6;

	int new_center_h = 0;
	int new_center_w = 0;

	float tmp = 0;
	float sad_val = 0;

	int ind_i = 0;
	int ind_j = 0;

	float min_sad = 20000000;
	float min_sad2 = 20000000;

	int center_ww = windowsize_w;

	// Mask
	BYTE2Dgray imgL;
	if(FALSE == make2DBYTEarray(&imgL, overlap_width, noheight))
	{
		return MEMORY_FAIL;
	}

	for(i = 0; i < noheight; i++)
	{
		for(j = 0; j < overlap_width; j++)
		{
			imgL.data[i][j] = (limg[i][j + nowidth - overlap_width].r +
				limg[i][j + nowidth - overlap_width].g + limg[i][j + nowidth - overlap_width].b) / 3;
		}
	}

	for(i = -thre_h; i < thre_h + 1; i++)
	{
		new_center_h = center_h + i;

		for(j = 0; j < thre_w + 1; j++)
		{
			new_center_w = center_ww + j;

			sad_val = 0;
			for(m = -windowsize_h; m < windowsize_h + 1; m++)
			{
				for(n = -windowsize_w; n < windowsize_w + 1; n++)
				{
					tmp = (float)(imgL.data[center_h + m][center_w + n]) - 
						(float)((rimg[new_center_h + m][new_center_w + n].r + 
						rimg[new_center_h + m][new_center_w + n].g + 
						rimg[new_center_h + m][new_center_w + n].b) / 3);
					tmp = abs(tmp);
					sad_val += tmp;
				}
			}

			if(sad_val < min_sad)
			{
				min_sad = sad_val;
				ind_i = i;
				ind_j = j;
			}
			else if((sad_val > min_sad) && (sad_val < min_sad2))
			{
				min_sad2 = sad_val;
			}
		}
	}

	if(min_sad / min_sad2 * 100.0 > 99.5)
	{
		ind_i = 0;
		ind_j = 0;
	}

	deleteBYTE2D(&imgL);

	*out_tx = (float)overlap_width - (2 * windowsize_w + 1) - (ind_j);
	*out_ty = (float)(-ind_i);

	return FALSE;
}

int computeTransform(BYTE2Dgray *limage, BYTE2Dgray *rimage, Point2D32f *featurePointsL, Point2D32f *featurePointsR,
					 int nPointsL, int nPointsR, int overlap_width, Float2Dgray *transformation)
{
	int nowidth = rimage->width;
	int noheight = rimage->height;

	int i;

	int *putative1;
	const float thres = 30.0f;

	int iter;
	Float2Dgray mat;
	float besta, bestb, besttx, bestty;
	int max_inliers = 0;
	int counter;
	float a, b, tx, ty;
	Float2Dgray matA, matb, mat_temp1, mat_temp2, rst;

	unsigned char **limagedata = limage->data, **rimagedata = rimage->data;
	float **transformationdata = transformation->data;

	int *real_putative1 = (int *)calloc(nPointsL, sizeof(int));
	if(real_putative1 == NULL)	//add.jw.2011-01-12
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, nPointsL);
		return MEMORY_FAIL;
	}

	int real_putative_num;

	DBG_PRINTF_SDH_DEBUG("Number of CornerLeft = %d , Right = %d\n", nPointsL, nPointsR);

	besta = bestb = besttx = bestty = 0;

	////////////////////////////////////
	// Exception - too few feature
	////////////////////////////////////
	if(nPointsL <= 5 || nPointsR <= 5)
	{
		DBG_PRINTF_SDH_DEBUG("Exception - too few feature\n");

		findmotionvector(limagedata, rimagedata, nowidth, noheight, overlap_width, &tx, &ty);
		transformationdata[0][0] = 1.0f;
		transformationdata[0][1] = 0.0f;
		transformationdata[0][2] = tx;

		transformationdata[1][0] = 0.0f;
		transformationdata[1][1] = 1.0f;
		transformationdata[1][2] = ty;

		transformationdata[2][0] = 0.0f;
		transformationdata[2][1] = 0.0f;
		transformationdata[2][2] = 1.0f;

		free(featurePointsL);
		free(featurePointsR);
		return FALSE;
	}

	real_putative1 = (int *)calloc(nPointsL, sizeof(int));
	if(real_putative1 == NULL)	//add.jw.2011-01-12
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, nPointsL);
		return MEMORY_FAIL;
	}

	putative1 = computePutativeCorrespondence(nPointsL, nPointsR, featurePointsL, featurePointsR, limage, rimage,
		overlap_width, real_putative1, &real_putative_num);
	if(NULL == putative1)
	{
		return MEMORY_FAIL;
	}

	if(FALSE == make2Dfloatarray(&mat, 4, 4))
	{
		return MEMORY_FAIL;
	}

	for(iter = 0; iter < 3000; iter++)
	{
		int prop1 = rand() % real_putative_num;
		int prop2 = rand() % real_putative_num;

		float x1, y1, x2, y2, x1t, y1t, x2t, y2t;
		int n_inliers = 0;

		x1 = featurePointsL[real_putative1[prop1]].x;
		y1 = featurePointsL[real_putative1[prop1]].y;
		x1t = featurePointsR[putative1[real_putative1[prop1]]].x;
		y1t = featurePointsR[putative1[real_putative1[prop1]]].y;

		x2 = featurePointsL[real_putative1[prop2]].x;
		y2 = featurePointsL[real_putative1[prop2]].y;
		x2t = featurePointsR[putative1[real_putative1[prop2]]].x;
		y2t = featurePointsR[putative1[real_putative1[prop2]]].y;

		mat.data[0][0] = x1;
		mat.data[0][1] = y1;
		mat.data[0][2] = 1.0f;
		mat.data[0][3] = 0.0f;

		mat.data[1][0] = y1;
		mat.data[1][1] = -x1;
		mat.data[1][2] = 0.0f;
		mat.data[1][3] = 1.0f;

		mat.data[2][0] = x2;
		mat.data[2][1] = y2;
		mat.data[2][2] = 1.0f;
		mat.data[2][3] = 0.0f;

		mat.data[3][0] = y2;
		mat.data[3][1] = -x2;
		mat.data[3][2] = 0.0f;
		mat.data[3][3] = 1.0f;

		invert(&mat);

		a = mat.data[0][0] * x1t + mat.data[0][1] * y1t + mat.data[0][2] * x2t + mat.data[0][3] * y2t;
		b = mat.data[1][0] * x1t + mat.data[1][1] * y1t + mat.data[1][2] * x2t + mat.data[1][3] * y2t;
		if(a * a + b * b < 0.99 || a * a + b * b > 1.01)
		{
			continue;
		}

		tx = mat.data[2][0] * x1t + mat.data[2][1] * y1t + mat.data[2][2] * x2t + mat.data[2][3] * y2t;
		ty = mat.data[3][0] * x1t + mat.data[3][1] * y1t + mat.data[3][2] * x2t + mat.data[3][3] * y2t;

		n_inliers = 0;
		for(i = 0; i < real_putative_num; i++)
		{
			float dist;
			x1 = (featurePointsL[real_putative1[i]].x);
			y1 = (featurePointsL[real_putative1[i]].y);

			x1t = featurePointsR[putative1[real_putative1[i]]].x;
			y1t = featurePointsR[putative1[real_putative1[i]]].y;

			dist = SQUARE(x1t - a * x1 - b * y1 - tx) + SQUARE(y1t + b * x1 - a * y1 - ty);

			if(dist < thres)
			{
				n_inliers++;
			}
		}

		if(n_inliers > max_inliers)
		{
			max_inliers = n_inliers;
			besta = a;
			bestb = b;
			besttx = tx;
			bestty = ty;
		}
	}

	deletefloat2D(&mat);

	DBG_PRINTF_SDH_DEBUG("Matched Points = %d\n", max_inliers);

	//////////////////////////////////////////
	// Exception - too few match point
	//////////////////////////////////////////
	if(max_inliers < 4)
	{
		DBG_PRINTF_SDH_DEBUG("Exception - too few match point\n");

		findmotionvector(limagedata, rimagedata, nowidth, noheight, overlap_width, &tx, &ty);
		transformationdata[0][0] = 1.0f;
		transformationdata[0][1] = 0.0f;
		transformationdata[0][2] = tx;

		transformationdata[1][0] = 0.0f;
		transformationdata[1][1] = 1.0f;
		transformationdata[1][2] = ty;

		transformationdata[2][0] = 0.0f;
		transformationdata[2][1] = 0.0f;
		transformationdata[2][2] = 1.0f;

		free(real_putative1);

		free(putative1);
		free(featurePointsL);
		free(featurePointsR);
		return max_inliers;
	}

	a = besta;
	b = bestb;
	tx = besttx;
	ty = bestty;

	if(FALSE == make2Dfloatarray(&matA, 4, 2 * max_inliers))
	{
		return MEMORY_FAIL;
	}

	if(FALSE == make2Dfloatarray(&matb, 1, 2 * max_inliers))
	{
		return MEMORY_FAIL;
	}

	if(FALSE == make2Dfloatarray(&mat_temp1, 4, 4))
	{
		return MEMORY_FAIL;
	}

	if(FALSE == make2Dfloatarray(&mat_temp2, 1, 4))
	{
		return MEMORY_FAIL;
	}

	if(FALSE == make2Dfloatarray(&rst, 1, 4))
	{
		return MEMORY_FAIL;
	}

	counter = 0;
	for(i = 0; i < real_putative_num; i++)
	{
		float x1, y1, x1t, y1t;
		float dist;

		x1 = (featurePointsL[real_putative1[i]].x);
		y1 = (featurePointsL[real_putative1[i]].y);

		x1t = (featurePointsR[putative1[real_putative1[i]]].x);
		y1t = (featurePointsR[putative1[real_putative1[i]]].y);

		dist = SQUARE(x1t - a * x1 - b * y1 - tx) + SQUARE(y1t + b * x1 - a * y1 - ty);

		if(dist < thres)
		{
			matA.data[counter][0] = x1;
			matA.data[counter][1] = y1;
			matA.data[counter][2] = 1;
			matA.data[counter][3] = 0;

			matb.data[counter][0] = x1t;

			counter++;

			matA.data[counter][0] = y1;
			matA.data[counter][1] = -x1;
			matA.data[counter][2] = 0;
			matA.data[counter][3] = 1;

			matb.data[counter][0] = y1t;
			counter++;
		}
	}

	if(MEMORY_FAIL == matMul(&matA, &matA, &mat_temp1, 1, 0))
	{
		return MEMORY_FAIL;
	}

	invert(&mat_temp1);

	if(MEMORY_FAIL == matMul(&matA, &matb, &mat_temp2, 1, 0))
	{
		return MEMORY_FAIL;
	}

	if(MEMORY_FAIL == matMul(&mat_temp1, &mat_temp2, &rst, 0, 0))
	{
		return MEMORY_FAIL;
	}

	deletefloat2D(&matA);
	deletefloat2D(&matb);
	deletefloat2D(&mat_temp1);
	deletefloat2D(&mat_temp2);

	a = rst.data[0][0];
	b = rst.data[1][0];
	tx = rst.data[2][0];
	ty = rst.data[3][0];
	deletefloat2D(&rst);

	DBG_PRINTF_SDH_DEBUG("Scale = %lf\n", a * a + b * b);
	transformationdata[0][0] = a;
	transformationdata[0][1] = b;
	transformationdata[0][2] = tx;

	transformationdata[1][0] = -b;
	transformationdata[1][1] = a;
	transformationdata[1][2] = ty;

	transformationdata[2][0] = 0.0f;
	transformationdata[2][1] = 0.0f;
	transformationdata[2][2] = 1.0f;
	invert(transformation);

	free(featurePointsL);
	free(featurePointsR);
	free(putative1);

	free(real_putative1);

	return max_inliers;
}

int computeTransformColor(BYTE2Dcolor *limage, BYTE2Dcolor *rimage, Point2D32f *featurePointsL, Point2D32f *featurePointsR,
					 int nPointsL, int nPointsR, int overlap_width, Float2Dgray *transformation)
{
	int nowidth = rimage->width;
	int noheight = rimage->height;

	int i;

	int *putative1;
	const float thres = 30.0f;

	int iter;
	Float2Dgray mat;
	float besta, bestb, besttx, bestty;
	int max_inliers = 0;
	int counter;
	float a, b, tx, ty;
	Float2Dgray matA, matb, mat_temp1, mat_temp2, rst;

	RgbPixel **limagedata = limage->data, **rimagedata = rimage->data;
	float **transformationdata = transformation->data;

	int *real_putative1 = (int *)calloc(nPointsL, sizeof(int));
	if(real_putative1 == NULL)	//add.jw.2011-01-12
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, nPointsL);
		return MEMORY_FAIL;
	}

	int real_putative_num;

	DBG_PRINTF_SDH_DEBUG("Number of CornerLeft = %d , Right = %d\n", nPointsL, nPointsR);

	besta = bestb = besttx = bestty = 0;

	////////////////////////////////////
	// Exception - too few feature
	////////////////////////////////////
	if(nPointsL <= 5 || nPointsR <= 5)
	{
		DBG_PRINTF_SDH_DEBUG("Exception - too few feature\n");

		findmotionvectorColor(limagedata, rimagedata, nowidth, noheight, overlap_width, &tx, &ty);
		transformationdata[0][0] = 1.0f;
		transformationdata[0][1] = 0.0f;
		transformationdata[0][2] = tx;

		transformationdata[1][0] = 0.0f;
		transformationdata[1][1] = 1.0f;
		transformationdata[1][2] = ty;

		transformationdata[2][0] = 0.0f;
		transformationdata[2][1] = 0.0f;
		transformationdata[2][2] = 1.0f;

		free(featurePointsL);
		free(featurePointsR);
		return FALSE;
	}

	real_putative1 = (int *)calloc(nPointsL, sizeof(int));
	if(real_putative1 == NULL)	//add.jw.2011-01-12
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, nPointsL);
		return MEMORY_FAIL;
	}

	putative1 = computePutativeCorrespondenceColor(nPointsL, nPointsR, featurePointsL, featurePointsR, limage, rimage,
		overlap_width, real_putative1, &real_putative_num);
	if(NULL == putative1)
	{
		return MEMORY_FAIL;
	}

	if(FALSE == make2Dfloatarray(&mat, 4, 4))
	{
		return MEMORY_FAIL;
	}

	for(iter = 0; iter < 3000; iter++)
	{
		int prop1 = rand() % real_putative_num;
		int prop2 = rand() % real_putative_num;

		float x1, y1, x2, y2, x1t, y1t, x2t, y2t;
		int n_inliers = 0;

		x1 = featurePointsL[real_putative1[prop1]].x;
		y1 = featurePointsL[real_putative1[prop1]].y;
		x1t = featurePointsR[putative1[real_putative1[prop1]]].x;
		y1t = featurePointsR[putative1[real_putative1[prop1]]].y;

		x2 = featurePointsL[real_putative1[prop2]].x;
		y2 = featurePointsL[real_putative1[prop2]].y;
		x2t = featurePointsR[putative1[real_putative1[prop2]]].x;
		y2t = featurePointsR[putative1[real_putative1[prop2]]].y;

		mat.data[0][0] = x1;
		mat.data[0][1] = y1;
		mat.data[0][2] = 1.0f;
		mat.data[0][3] = 0.0f;

		mat.data[1][0] = y1;
		mat.data[1][1] = -x1;
		mat.data[1][2] = 0.0f;
		mat.data[1][3] = 1.0f;

		mat.data[2][0] = x2;
		mat.data[2][1] = y2;
		mat.data[2][2] = 1.0f;
		mat.data[2][3] = 0.0f;

		mat.data[3][0] = y2;
		mat.data[3][1] = -x2;
		mat.data[3][2] = 0.0f;
		mat.data[3][3] = 1.0f;

		invert(&mat);

		a = mat.data[0][0] * x1t + mat.data[0][1] * y1t + mat.data[0][2] * x2t + mat.data[0][3] * y2t;
		b = mat.data[1][0] * x1t + mat.data[1][1] * y1t + mat.data[1][2] * x2t + mat.data[1][3] * y2t;
		if(a * a + b * b < 0.99 || a * a + b * b > 1.01)
		{
			continue;
		}

		tx = mat.data[2][0] * x1t + mat.data[2][1] * y1t + mat.data[2][2] * x2t + mat.data[2][3] * y2t;
		ty = mat.data[3][0] * x1t + mat.data[3][1] * y1t + mat.data[3][2] * x2t + mat.data[3][3] * y2t;

		n_inliers = 0;
		for(i = 0; i < real_putative_num; i++)
		{
			float dist;
			x1 = (featurePointsL[real_putative1[i]].x);
			y1 = (featurePointsL[real_putative1[i]].y);

			x1t = featurePointsR[putative1[real_putative1[i]]].x;
			y1t = featurePointsR[putative1[real_putative1[i]]].y;

			dist = SQUARE(x1t - a * x1 - b * y1 - tx) + SQUARE(y1t + b * x1 - a * y1 - ty);

			if(dist < thres)
			{
				n_inliers++;
			}
		}

		if(n_inliers > max_inliers)
		{
			max_inliers = n_inliers;
			besta = a;
			bestb = b;
			besttx = tx;
			bestty = ty;
		}
	}

	deletefloat2D(&mat);

	DBG_PRINTF_SDH_DEBUG("Matched Points = %d\n", max_inliers);

	//////////////////////////////////////////
	// Exception - too few match point
	//////////////////////////////////////////
	if(max_inliers < 4)
	{
		DBG_PRINTF_SDH_DEBUG("Exception - too few match point\n");

		findmotionvectorColor(limagedata, rimagedata, nowidth, noheight, overlap_width, &tx, &ty);
		transformationdata[0][0] = 1.0f;
		transformationdata[0][1] = 0.0f;
		transformationdata[0][2] = tx;

		transformationdata[1][0] = 0.0f;
		transformationdata[1][1] = 1.0f;
		transformationdata[1][2] = ty;

		transformationdata[2][0] = 0.0f;
		transformationdata[2][1] = 0.0f;
		transformationdata[2][2] = 1.0f;

		free(real_putative1);

		free(putative1);
		free(featurePointsL);
		free(featurePointsR);
		return max_inliers;
	}

	a = besta;
	b = bestb;
	tx = besttx;
	ty = bestty;

	if(FALSE == make2Dfloatarray(&matA, 4, 2 * max_inliers))
	{
		return MEMORY_FAIL;
	}

	if(FALSE == make2Dfloatarray(&matb, 1, 2 * max_inliers))
	{
		return MEMORY_FAIL;
	}

	if(FALSE == make2Dfloatarray(&mat_temp1, 4, 4))
	{
		return MEMORY_FAIL;
	}

	if(FALSE == make2Dfloatarray(&mat_temp2, 1, 4))
	{
		return MEMORY_FAIL;
	}

	if(FALSE == make2Dfloatarray(&rst, 1, 4))
	{
		return MEMORY_FAIL;
	}

	counter = 0;
	for(i = 0; i < real_putative_num; i++)
	{
		float x1, y1, x1t, y1t;
		float dist;

		x1 = (featurePointsL[real_putative1[i]].x);
		y1 = (featurePointsL[real_putative1[i]].y);

		x1t = (featurePointsR[putative1[real_putative1[i]]].x);
		y1t = (featurePointsR[putative1[real_putative1[i]]].y);

		dist = SQUARE(x1t - a * x1 - b * y1 - tx) + SQUARE(y1t + b * x1 - a * y1 - ty);

		if(dist < thres)
		{
			matA.data[counter][0] = x1;
			matA.data[counter][1] = y1;
			matA.data[counter][2] = 1;
			matA.data[counter][3] = 0;

			matb.data[counter][0] = x1t;

			counter++;

			matA.data[counter][0] = y1;
			matA.data[counter][1] = -x1;
			matA.data[counter][2] = 0;
			matA.data[counter][3] = 1;

			matb.data[counter][0] = y1t;
			counter++;
		}
	}

	if(MEMORY_FAIL == matMul(&matA, &matA, &mat_temp1, 1, 0))
	{
		return MEMORY_FAIL;
	}

	invert(&mat_temp1);

	if(MEMORY_FAIL == matMul(&matA, &matb, &mat_temp2, 1, 0))
	{
		return MEMORY_FAIL;
	}

	if(MEMORY_FAIL == matMul(&mat_temp1, &mat_temp2, &rst, 0, 0))
	{
		return MEMORY_FAIL;
	}

	deletefloat2D(&matA);
	deletefloat2D(&matb);
	deletefloat2D(&mat_temp1);
	deletefloat2D(&mat_temp2);

	a = rst.data[0][0];
	b = rst.data[1][0];
	tx = rst.data[2][0];
	ty = rst.data[3][0];
	deletefloat2D(&rst);

	DBG_PRINTF_SDH_DEBUG("Scale = %lf\n", a * a + b * b);
	transformationdata[0][0] = a;
	transformationdata[0][1] = b;
	transformationdata[0][2] = tx;

	transformationdata[1][0] = -b;
	transformationdata[1][1] = a;
	transformationdata[1][2] = ty;

	transformationdata[2][0] = 0.0f;
	transformationdata[2][1] = 0.0f;
	transformationdata[2][2] = 1.0f;
	invert(transformation);

	free(featurePointsL);
	free(featurePointsR);
	free(putative1);

	free(real_putative1);

	return max_inliers;
}

void getStitchingCut(UINT2Dgray *DMap, UINT2Dgray *Acc, int *CutPos, int nWidth, int nHeight)
{
#define __INFINITY	2000000000
	int vert, horz, min_pos;
	UINT min_cost, min_cost_pos;
	UINT **DMapdata = DMap->data, **Accdata = Acc->data;

	//int min_gap, gap_l, gap_r;
	memcpy(Accdata[0], DMapdata[0], sizeof(UINT) * nWidth);

	for(vert = 1; vert < nHeight; vert++)
	{
		Accdata[vert][0] = Accdata[vert][nWidth - 1] = __INFINITY;
		for(horz = 1; horz < nWidth - 1; horz++)
		{
			Accdata[vert][horz] = DMapdata[vert][horz] + min(Accdata[(vert - 1)][horz - 1],
				min(Accdata[(vert - 1)][horz], Accdata[(vert - 1)][horz + 1]));
		}
	}

	min_cost = __INFINITY;
	min_cost_pos = 0;
	for(horz = 1; horz < nWidth - 1; horz++)
	{
		if(Accdata[(nHeight - 1)][horz] < min_cost)
		{
			min_cost_pos = horz;
			min_cost = Accdata[(nHeight - 1)][horz];
		}
	}

	CutPos[nHeight - 1] = min_cost_pos;
	min_pos = nWidth;

	//DBG_PRINTF_SDH_DEBUG("%d\n", min_cost_pos);
	for(vert = nHeight - 2; vert >= 0; vert--)
	{
		UINT diff = Accdata[(vert + 1)][min_cost_pos] - DMapdata[(vert + 1)][min_cost_pos];

		UINT *ptr = Accdata[vert] + min_cost_pos;

		if(diff == *(ptr - 1))
		{
			min_cost_pos -= 1;
		}
		else if(diff == *(ptr + 1))
		{
			min_cost_pos += 1;
		}
		//else
		//{
		//	ASSERT( diff == *(ptr) );
		//}
		
		CutPos[vert] = min_cost_pos;

		//gap_l = (int)min_cost_pos - boundary[vert];
		//gap_r = nWidth - (int)min_cost_pos - 1;
		//if(gap_l > 0)
		//	min_gap = min(gap_l, gap_r);
		//else
		//	min_gap = gap_r;
		//if(min_pos > min_gap)
		//	min_pos = min_gap;
	}

	//return min_pos;
}

void overlap_boundary(BYTE2Dgray *imgL, int *boundary, int nWidth, int nHeight, int LL)
{
	int i, j;
	int tmp = nWidth - 1;
	for(i = 0; i < nHeight; i++)
	{
		j = LL;
		while(imgL->data[i][j] == WHITE_VALUE)
		{
			j++;
			if(j == tmp)
			{
				break;
			}
		}

		boundary[i] = j - LL;
	}
}

void overlap_boundaryColor(BYTE2Dcolor *imgL, int *boundary, int nWidth, int nHeight, int LL)
{
	int i, j;
	int tmp = nWidth - 1;
	for(i = 0; i < nHeight; i++)
	{
		j = LL;
		while(imgL->data[i][j].r == WHITE_VALUE)
		{
			j++;
			if(j == tmp)
			{
				break;
			}
		}

		boundary[i] = j - LL;
	}
}

int Qsort_compair(const void *a, const void *b)
{
	return((*(int *)a) - (*((int *)b)));
}

int blending(BYTE2Dgray *imgL, BYTE2Dgray *imgL_tmp, int width, int height, int LL, int LL2, int blendtype) //void->int. mod.bong.2010-11-25 10:34AM
{
#if 0
	UINT2Dgray CostMatrix, AccCostMatrix;
	int *CutPos;
	int *boundary;
	Float2Dgray gaussian_weight, gaussian_weight_tmp;
	BYTE2Dgray blend_result;
	int *array4;
	int iter;
	int offset;
	short *gap;
	float tmp_out;
	Float2Dgray imgL_high, imgL_low, imgR_high, imgR_low, blend_result_f;
	Short2Dgray imgL_dx, imgL_dy, imgR_dx, imgR_dy, b;
	BYTE2Dgray tmp_result[2];
#endif
	int horz, vert;
	float *weight;
	int blend_w;
	int LL3;
	unsigned char **imgL_data = imgL->data;
	unsigned char **imgL_tmp_data = imgL_tmp->data;
	switch(blendtype)
	{
#if 0
	case 0:	// Non blending
		if(FALSE == make2DUINTarray(&CostMatrix, width - LL + 1, height))
		{
			return MEMORY_FAIL;
		}

		if(FALSE == make2DUINTarray(&AccCostMatrix, width - LL + 1, height))
		{
			return MEMORY_FAIL;
		}

		CutPos = (int *)calloc(height, sizeof(int));
		if(CutPos == NULL)	//add.bong.2010-11-25 10:27AM
		{
			DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
			return MEMORY_FAIL;
		}

		for(vert = 0; vert < height; vert++)
		{
			for(horz = LL; horz < width; horz++)
			{
				CostMatrix.data[vert][horz - LL] = myabs((int)imgL_tmp_data[vert][horz - LL] - (int)imgL_data[vert][horz]);

				//DBG_PRINTF_SDH_DEBUG("%f\n",a);
			}
		}

		boundary = (int *)calloc(height, sizeof(int));
		if(boundary == NULL)	//add.bong.2010-11-25 10:27AM
		{
			DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
			return MEMORY_FAIL;
		}

		overlap_boundary(imgL, boundary, width, height, LL);
		getStitchingCut(&CostMatrix, &AccCostMatrix, CutPos, CostMatrix.width, CostMatrix.height);

		free(CostMatrix.data[0]);
		free(CostMatrix.data);
		free(AccCostMatrix.data[0]);
		free(AccCostMatrix.data);

		for(vert = 0; vert < height; vert++)
		{
			for(horz = LL; horz <= CutPos[vert] + LL; horz++)
			{
				//if( Cut[vert - 0][horz - width] == 0x00ff0000 )
				imgL_data[vert][horz] = imgL_tmp_data[vert][horz - LL];
				//else
				//	dst[vert][horz] = img2[vert][horz];
			}

			//imgL_data[vert][horz] = 0;
		}

		deleteBYTE2D(imgL_tmp);
		free(CutPos);

		//free(boundary);
		break;
#endif
	case 1:	// Linear blending
		blend_w = width - LL2;
		weight = (float *)calloc(blend_w, sizeof(float));
		if(weight == NULL)	//add.bong.2010-11-25 10:27AM
		{
			DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, blend_w);
			return MEMORY_FAIL;
		}

		for(horz = 0; horz < blend_w; horz++)
		{
			weight[horz] = (float)horz / (float)(blend_w - 1);
		}

		LL3 = LL2 - LL;

		//DBG_PRINTF_SDH_DEBUG("%d\n", LL3);
		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < LL3; horz++)
			{
				imgL_data[vert][horz + LL] = imgL_tmp_data[vert][horz];
			}
		}

		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < blend_w; horz++)
			{
				imgL_data[vert][horz + LL2] = (unsigned char)(((float)imgL_tmp_data[vert][horz + LL3] * (1.0f - weight[horz])) +
					(float)imgL_data[vert][horz + LL2] * weight[horz] + 0.5f);
				//imgL_data[vert][horz + LL2] = (unsigned char)((float)imgL_tmp_data[vert][horz + LL3] + ((float)imgL_data[vert][horz + LL2] - (float)imgL_tmp_data[vert][horz + LL3]) * weight[horz] + 0.5f);
				//(unsigned char)(((float)imgL_tmp_data[vert][horz + LL3] * (1.0f - weight[horz])) + (float)imgL_data[vert][horz + LL2] * weight[horz] + 0.5f);	
			}
		}

		deleteBYTE2D(imgL_tmp);
		free(weight);
		break;
#if 0
	case 2:	// Seam linear blending
		LL3 = (width - LL2) * 2 / 3;

		if(FALSE == make2DUINTarray(&CostMatrix, LL3, height))
		{
			return MEMORY_FAIL;
		}

		if(FALSE == make2DUINTarray(&AccCostMatrix, LL3, height))
		{
			return MEMORY_FAIL;
		}

		CutPos = (int *)calloc(height, sizeof(int));
		if(CutPos == NULL)	//add.bong.2010-11-25 10:27AM
		{
			DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
			return MEMORY_FAIL;
		}

		offset = LL2 + LL3 / 4;
		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < LL3; horz++)
			{
				CostMatrix.data[vert][horz] = myabs((int)imgL_tmp_data[vert][horz - LL + offset] - (int)imgL_data[vert][horz + offset]);

				//DBG_PRINTF_SDH_DEBUG("%f\n", a);
			}
		}

		//boundary = (int *)calloc(height, sizeof(int));
		//overlap_boundary(imgL,boundary, width, height, LL);
		getStitchingCut(&CostMatrix, &AccCostMatrix, CutPos, CostMatrix.width, CostMatrix.height);
		free(CostMatrix.data[0]);
		free(CostMatrix.data);
		free(AccCostMatrix.data[0]);
		free(AccCostMatrix.data);

		blend_w = LL3 / 4;
		if(blend_w % 2 == 0)
		{
			blend_w--;
		}

		DBG_PRINTF_SDH_DEBUG("Blending Width = %d\n", blend_w);
		if(blend_w < 2)
		{
			for(vert = 0; vert < height; vert++)
			{
				for(horz = LL; horz <= CutPos[vert] + offset; horz++)
				{
					imgL_data[vert][horz] = imgL_tmp_data[vert][horz - LL];
				}
			}
		}
		else
		{
			weight = (float *)calloc(blend_w, sizeof(float));
			if(weight == NULL)	//add.bong.2010-11-25 10:27AM
			{
				DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, blend_w);
				return MEMORY_FAIL;
			}

			for(horz = 0; horz < blend_w; horz++)
			{
				weight[horz] = (float)horz / (float)(blend_w - 1);
			}

			for(vert = 0; vert < height; vert++)
			{
				int count = 0;
				for(horz = LL; horz < CutPos[vert] + offset - blend_w / 2; horz++)
				{
					imgL_data[vert][horz] = imgL_tmp_data[vert][horz - LL];
				}

				for(horz = CutPos[vert] + offset - blend_w / 2; horz <= CutPos[vert] + offset + blend_w / 2; horz++)
				{
					imgL_data[vert][horz] = (unsigned char)((float)imgL_tmp_data[vert][horz - LL] + 
						((float)imgL_data[vert][horz] - (float)imgL_tmp_data[vert][horz - LL]) * weight[count] + 0.5f);
					//(unsigned char)(((float)imgL_tmp_data[vert][horz - LL] * (1.0f - weight[count])) + (float)imgL_data[vert][horz] * weight[count] + 0.5f);
					count++;
				}

				//imgL_data[vert][horz] = 0;
			}

			free(weight);
		}

		deleteBYTE2D(imgL_tmp);
		free(CutPos);

		//free(boundary);
		break;

	case 3:	// bright compensation
		LL3 = (width - LL2) / 2;

		if(FALSE == make2DUINTarray(&CostMatrix, LL3, height))
		{
			return MEMORY_FAIL;
		}

		if(FALSE == make2DUINTarray(&AccCostMatrix, LL3, height))
		{
			return MEMORY_FAIL;
		}

		CutPos = (int *)calloc(height, sizeof(int));
		if(CutPos == NULL)	//add.bong.2010-11-25 10:27AM
		{
			DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
			return MEMORY_FAIL;
		}

		offset = LL2 + LL3 / 2;
		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < LL3; horz++)
			{
				CostMatrix.data[vert][horz] = myabs((int)imgL_tmp_data[vert][horz - LL + offset] - (int)imgL_data[vert][horz + offset]);

				//DBG_PRINTF_SDH_DEBUG("%f\n", a);
			}
		}

		//boundary = (int *)calloc(height, sizeof(int));
		//overlap_boundary(imgL, boundary, width, height, LL);
		getStitchingCut(&CostMatrix, &AccCostMatrix, CutPos, CostMatrix.width, CostMatrix.height);
		free(CostMatrix.data[0]);
		free(CostMatrix.data);
		free(AccCostMatrix.data[0]);
		free(AccCostMatrix.data);

		blend_w = LL3 / 2;
		if(blend_w % 2 == 0)
		{
			blend_w--;
		}

		DBG_PRINTF_SDH_DEBUG("Blending Width = %d\n", blend_w);
		if(blend_w < 2)
		{
			for(vert = 0; vert < height; vert++)
			{
				for(horz = LL; horz <= CutPos[vert] + offset; horz++)
				{
					imgL_data[vert][horz] = imgL_tmp_data[vert][horz - LL];
				}
			}
		}
		else
		{
			gap = (short *)calloc(height, sizeof(short));
			if(gap == NULL) //add.bong.2010-11-25 10:27AM
			{
				DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
				return MEMORY_FAIL;
			}

			vert = 0;
			gap[vert] = imgL_tmp_data[vert][CutPos[vert] + offset - LL] - imgL_data[vert][CutPos[vert] + offset];
			for(vert = 1; vert < height - 1; vert++)
			{
				short gap1 = imgL_tmp_data[vert][CutPos[vert] + offset - LL - 1] - imgL_data[vert][CutPos[vert] + offset - 1];
				short gap2 = imgL_tmp_data[vert][CutPos[vert] + offset - LL] - imgL_data[vert][CutPos[vert] + offset];
				short gap3 = imgL_tmp_data[vert][CutPos[vert] + offset - LL + 1] - imgL_data[vert][CutPos[vert] + offset + 1];
				gap[vert] = (gap1 + gap3 + gap2) / 3;

				//short gap4 = min(myabs(gap1), min(myabs(gap2), myabs(gap3)));
				//gap[vert] = gap4;
				//if(gap4 != gap1 && gap4 != gap2 && gap4 != gap3)
				//	gap[vert] =- gap4;			
			}

			gap[vert] = imgL_tmp_data[vert][CutPos[vert] + offset - LL] - imgL_data[vert][CutPos[vert] + offset];

			weight = (float *)calloc(blend_w, sizeof(float));
			if(weight == NULL)	//add.bong.2010-11-25 10:27AM
			{
				DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, blend_w);
				return MEMORY_FAIL;
			}

			for(horz = 0; horz < blend_w; horz++)
			{
				weight[horz] = (float)horz / (float)(blend_w - 1);
			}

			for(vert = 0; vert < height; vert++)
			{
				int count = 0;
				for(horz = LL; horz < CutPos[vert] + offset - blend_w / 2; horz++)
				{
					imgL_data[vert][horz] = imgL_tmp_data[vert][horz - LL];
				}

				for(; horz < CutPos[vert] + offset; horz++)
				{
					tmp_out = (float)imgL_tmp_data[vert][horz - LL] - weight[count] * ((float)gap[vert]) + 0.5f;
					if(tmp_out > 255)
					{
						tmp_out = 255;
					}
					else if(tmp_out < 0)
					{
						tmp_out = 0;
					}

					imgL_data[vert][horz] = (unsigned char)tmp_out;
					count++;
				}

				for(; horz <= CutPos[vert] + offset + blend_w / 2; horz++)
				{
					tmp_out = (float)imgL_data[vert][horz] + (1.f - weight[count]) * ((float)gap[vert]) + 0.5f;
					if(tmp_out > 255)
					{
						tmp_out = 255;
					}
					else if(tmp_out < 0)
					{
						tmp_out = 0;
					}

					imgL_data[vert][horz] = (unsigned char)tmp_out;
					count++;
				}
			}

			free(weight);
			free(gap);
		}

		deleteBYTE2D(imgL_tmp);
		free(CutPos);

		//free(boundary);
		break;

	case 4:	// Two band blending
		LL3 = LL2 - LL;

		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < LL3; horz++)
			{
				imgL_data[vert][horz + LL] = imgL_tmp_data[vert][horz];
			}
		}

		blend_w = width - LL2;
		if(FALSE == make2Dfloatarray(&imgL_high, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		if(FALSE == make2Dfloatarray(&imgR_high, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < blend_w; horz++)
			{
				imgL_high.data[vert][horz] = (float)imgL_tmp_data[vert][horz + LL3];
				imgR_high.data[vert][horz] = (float)imgL_data[vert][horz + LL2];
			}
		}

		deleteBYTE2D(imgL_tmp);
		if(FALSE == make2Dfloatarray(&imgL_low, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		if(FALSE == make2Dfloatarray(&imgR_low, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		gaussian(&imgL_high, &imgL_low, 3.f);
		gaussian(&imgR_high, &imgR_low, 3.f);
		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < blend_w; horz++)
			{
				imgL_high.data[vert][horz] -= imgL_low.data[vert][horz];
				imgR_high.data[vert][horz] -= imgR_low.data[vert][horz];
			}
		}

		weight = (float *)calloc(blend_w, sizeof(float));
		if(weight == NULL)	//add.bong.2010-11-25 10:27AM
		{
			DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, blend_w);
			return MEMORY_FAIL;
		}

		for(horz = 0; horz < blend_w; horz++)
		{
			weight[horz] = (float)horz / (float)(blend_w - 1);
		}

		if(FALSE == make2Dfloatarray(&blend_result_f, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < blend_w; horz++)
			{
				blend_result_f.data[vert][horz] = imgL_low.data[vert][horz] + (imgR_low.data[vert][horz] - imgL_low.data[vert][horz]) * weight[horz];
			}
		}

		deletefloat2D(&imgR_low);
		deletefloat2D(&imgL_low);
		free(weight);

		if(FALSE == make2DUINTarray(&CostMatrix, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		if(FALSE == make2DUINTarray(&AccCostMatrix, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		CutPos = (int *)calloc(height, sizeof(int));
		if(CutPos == NULL)	//add.bong.2010-11-25 10:27AM
		{
			DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
			return MEMORY_FAIL;
		}

		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < blend_w; horz++)
			{
				CostMatrix.data[vert][horz] = myabs((int)imgL_high.data[vert][horz] - (int)imgR_high.data[vert][horz]);

				//DBG_PRINTF_SDH_DEBUG("%f\n", a);
			}
		}

		getStitchingCut(&CostMatrix, &AccCostMatrix, CutPos, CostMatrix.width, CostMatrix.height);
		free(CostMatrix.data[0]);
		free(CostMatrix.data);
		free(AccCostMatrix.data[0]);
		free(AccCostMatrix.data);
		for(vert = 0; vert < height; vert++)
		{
			for(horz = CutPos[vert]; horz < blend_w; horz++)
			{
				imgL_high.data[vert][horz] = imgR_high.data[vert][horz];
			}
		}

		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < blend_w; horz++)
			{
				int tmp = (int)(blend_result_f.data[vert][horz] + imgL_high.data[vert][horz] + 0.5f);
				if(tmp > 255)
				{
					tmp = 255;
				}
				else if(tmp < 0)
				{
					tmp = 0;
				}

				imgL_data[vert][horz + LL2] = (unsigned char)tmp;
			}
		}

		deletefloat2D(&imgL_high);
		deletefloat2D(&imgR_high);
		deletefloat2D(&blend_result_f);
		free(CutPos);
		break;

	case 5:	// Iterative GIST2
		LL3 = LL2 - LL;
		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < LL3; horz++)
			{
				imgL_data[vert][horz + LL] = imgL_tmp_data[vert][horz];
			}
		}

		blend_w = width - LL2;
		if(FALSE == make2Dshortarray(&imgL_dx, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		if(FALSE == make2Dshortarray(&imgL_dy, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		if(FALSE == make2Dshortarray(&imgR_dx, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		if(FALSE == make2Dshortarray(&imgR_dy, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		memset(imgL_dx.data[0], 0, blend_w * height * sizeof(short));
		memset(imgL_dy.data[0], 0, blend_w * height * sizeof(short));
		memset(imgR_dx.data[0], 0, blend_w * height * sizeof(short));
		memset(imgR_dy.data[0], 0, blend_w * height * sizeof(short));

		for(vert = 0; vert < height - 1; vert++)
		{
			for(horz = 0; horz < blend_w - 1; horz++)
			{
				imgL_dx.data[vert][horz] = imgL_tmp_data[vert][horz + LL3 + 1] - imgL_tmp_data[vert][horz + LL3];
				imgL_dy.data[vert][horz] = imgL_tmp_data[vert + 1][horz + LL3] - imgL_tmp_data[vert][horz + LL3];
				imgR_dx.data[vert][horz] = imgL_data[vert][horz + LL2 + 1] - imgL_data[vert][horz + LL2];
				imgR_dy.data[vert][horz] = imgL_data[vert + 1][horz + LL2] - imgL_data[vert][horz + LL2];
			}
		}

		if(FALSE == make2DUINTarray(&CostMatrix, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		if(FALSE == make2DUINTarray(&AccCostMatrix, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		CutPos = (int *)calloc(height, sizeof(int));
		if(CutPos == NULL)	//add.bong.2010-11-25 10:27AM
		{
			DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
			return MEMORY_FAIL;
		}

		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < blend_w; horz++)
			{
				CostMatrix.data[vert][horz] = (int)(myabs(imgL_dx.data[vert][horz] - imgR_dx.data[vert][horz]) + myabs(imgL_dy.data[vert][horz] - imgR_dy.data[vert][horz]));

				//DBG_PRINTF_SDH_DEBUG("%f\n", a);
			}
		}

		getStitchingCut(&CostMatrix, &AccCostMatrix, CutPos, CostMatrix.width, CostMatrix.height);
		free(CostMatrix.data[0]);
		free(CostMatrix.data);
		free(AccCostMatrix.data[0]);
		free(AccCostMatrix.data);

		for(vert = 0; vert < height; vert++)
		{
			for(horz = CutPos[vert]; horz < blend_w; horz++)
			{
				imgL_dx.data[vert][horz] = imgR_dx.data[vert][horz];
				imgL_dy.data[vert][horz] = imgR_dy.data[vert][horz];
			}
		}

		deleteshort2D(&imgR_dx);
		deleteshort2D(&imgR_dy);
		free(CutPos);

		weight = (float *)calloc(blend_w, sizeof(float));
		if(weight == NULL)	//add.bong.2010-11-25 10:27AM
		{
			DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, blend_w);
			return MEMORY_FAIL;
		}

		for(horz = 0; horz < blend_w; horz++)
		{
			weight[horz] = (float)horz / (float)(blend_w - 1);
		}

		if(FALSE == make2DBYTEarray(&blend_result, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < blend_w; horz++)
			{
				float tmp = (float)imgL_tmp_data[vert][horz + LL3];
				blend_result.data[vert][horz] = (unsigned char)(tmp + ((float)imgL_data[vert][horz + LL2] - tmp) * weight[horz] + 0.5f);
			}
		}

		deleteBYTE2D(imgL_tmp);
		free(weight);

		array4 = (int *)calloc(4, sizeof(int));
		if(array4 == NULL)	//add.bong.2010-11-25 10:27AM
		{
			DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, 4);
			return MEMORY_FAIL;
		}

		for(iter = 0; iter < 10; iter++)
		{
			for(vert = 1; vert < height - 1; vert++)
			{
				for(horz = 1; horz < blend_w - 1; horz++)
				{
					int tmp_median;
					array4[0] = (int)(blend_result.data[vert][horz + 1] - imgL_dx.data[vert][horz]);
					array4[1] = (int)(blend_result.data[vert][horz - 1] + imgL_dx.data[vert][horz - 1]);
					array4[2] = (int)(blend_result.data[vert + 1][horz] - imgL_dy.data[vert][horz]);
					array4[3] = (int)(blend_result.data[vert - 1][horz] + imgL_dy.data[vert - 1][horz]);
					qsort((void *)array4, 4, sizeof(int), Qsort_compair);
					tmp_median = (array4[1] + array4[2] + 1) / 2;
					if(tmp_median > 255)
					{
						tmp_median = 255;
					}
					else if(tmp_median < 0)
					{
						tmp_median = 0;
					}

					blend_result.data[vert][horz] = (unsigned char)tmp_median;
				}
			}
		}

		deleteshort2D(&imgL_dx);
		deleteshort2D(&imgL_dy);
		free(array4);
		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < blend_w; horz++)
			{
				imgL_data[vert][horz + LL2] = blend_result.data[vert][horz];
			}
		}

		deleteBYTE2D(&blend_result);
		break;

	case 6:	// GIST2 - Successive Over-relaxation
		LL3 = LL2 - LL;
		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < LL3; horz++)
			{
				imgL_data[vert][horz + LL] = imgL_tmp_data[vert][horz];
			}
		}

		blend_w = width - LL2;
		if(FALSE == make2Dshortarray(&imgL_dx, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		if(FALSE == make2Dshortarray(&imgL_dy, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		if(FALSE == make2Dshortarray(&imgR_dx, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		if(FALSE == make2Dshortarray(&imgR_dy, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		memset(imgL_dx.data[0], 0, blend_w * height * sizeof(short));
		memset(imgL_dy.data[0], 0, blend_w * height * sizeof(short));
		memset(imgR_dx.data[0], 0, blend_w * height * sizeof(short));
		memset(imgR_dy.data[0], 0, blend_w * height * sizeof(short));

		for(vert = 0; vert < height - 1; vert++)
		{
			for(horz = 0; horz < blend_w - 1; horz++)
			{
				imgL_dx.data[vert][horz] = imgL_tmp_data[vert][horz + LL3 + 1] - imgL_tmp_data[vert][horz + LL3];
				imgL_dy.data[vert][horz] = imgL_tmp_data[vert + 1][horz + LL3] - imgL_tmp_data[vert][horz + LL3];
				imgR_dx.data[vert][horz] = imgL_data[vert][horz + LL2 + 1] - imgL_data[vert][horz + LL2];
				imgR_dy.data[vert][horz] = imgL_data[vert + 1][horz + LL2] - imgL_data[vert][horz + LL2];
			}
		}

		if(FALSE == make2DUINTarray(&CostMatrix, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		if(FALSE == make2DUINTarray(&AccCostMatrix, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		CutPos = (int *)calloc(height, sizeof(int));
		if(CutPos == NULL)	//add.bong.2010-11-25 10:27AM
		{
			DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
			return MEMORY_FAIL;
		}

		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < blend_w; horz++)
			{
				CostMatrix.data[vert][horz] = (int)(myabs(imgL_dx.data[vert][horz] - imgR_dx.data[vert][horz]) + myabs(imgL_dy.data[vert][horz] - imgR_dy.data[vert][horz]));

				//DBG_PRINTF_SDH_DEBUG("%f\n", a);
			}
		}

		getStitchingCut(&CostMatrix, &AccCostMatrix, CutPos, CostMatrix.width, CostMatrix.height);
		free(CostMatrix.data[0]);
		free(CostMatrix.data);
		free(AccCostMatrix.data[0]);
		free(AccCostMatrix.data);

		for(vert = 0; vert < height; vert++)
		{
			for(horz = CutPos[vert] + 1; horz < blend_w; horz++)
			{
				imgL_dx.data[vert][horz] = imgR_dx.data[vert][horz];
				imgL_dy.data[vert][horz] = imgR_dy.data[vert][horz];
			}
		}

		deleteshort2D(&imgR_dx);
		deleteshort2D(&imgR_dy);
		free(CutPos);
		if(FALSE == make2Dshortarray(&b, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		// make b array
		memset(b.data[0], 0, sizeof(short) * blend_w * height);
		for(vert = 1; vert < height - 1; vert++)
		{
			for(horz = 1; horz < blend_w - 1; horz++)
			{
				b.data[vert][horz] = imgL_dx.data[vert][horz - 1] - imgL_dx.data[vert][horz] -
					imgL_dy.data[vert][horz] + imgL_dy.data[vert - 1][horz];
			}
		}

		deleteshort2D(&imgL_dx);
		deleteshort2D(&imgL_dy);

		weight = (float *)calloc(blend_w, sizeof(float));
		if(weight == NULL)	//add.bong.2010-11-25 10:27AM
		{
			DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, blend_w);
			return MEMORY_FAIL;
		}

		for(horz = 0; horz < blend_w; horz++)
		{
			weight[horz] = (float)horz / (float)(blend_w - 1);
		}

		if(FALSE == make2DBYTEarray(tmp_result, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < blend_w; horz++)
			{
				float tmp = (float)imgL_tmp_data[vert][horz + LL3];
				tmp_result[0].data[vert][horz] = (unsigned char)(tmp + ((float)imgL_data[vert][horz + LL2] - tmp) * weight[horz] + 0.5f);
			}
		}

		free(weight);
		deleteBYTE2D(imgL_tmp);

		if(FALSE == make2DBYTEarray(tmp_result + 1, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		memcpy(tmp_result[1].data[0], tmp_result[0].data[0], sizeof(unsigned char) * height * blend_w);

		for(iter = 0; iter < 10; iter++)
		{
			int count = iter % 2;
			int sigma;
			for(vert = 1; vert < height - 1; vert++)
			{
				for(horz = 1; horz < blend_w - 1; horz++)
				{
					int tmp;
					sigma = -tmp_result[(count + 1) % 2].data[vert - 1][horz];
					sigma -= tmp_result[(count + 1) % 2].data[vert][horz - 1];
					sigma -= tmp_result[count].data[vert + 1][horz];
					sigma -= tmp_result[count].data[vert][horz + 1];

					tmp = tmp_result[count].data[vert][horz] + ((b.data[vert][horz] - sigma + 2) / 4 - tmp_result[count].data[vert][horz]) * 3 / 2;
					if(tmp > 255)
					{
						tmp = 255;
					}
					else if(tmp < 0)
					{
						tmp = 0;
					}

					tmp_result[(count + 1) % 2].data[vert][horz] = (unsigned char)tmp;
				}
			}
		}

		deleteshort2D(&b);

		iter %= 2;
		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < blend_w; horz++)
			{
				imgL_data[vert][horz + LL2] = tmp_result[iter].data[vert][horz];
			}
		}

		deleteBYTE2D(tmp_result);
		deleteBYTE2D(tmp_result + 1);
		break;

	case 7:	// Gaussian blending
		if(FALSE == make2DUINTarray(&CostMatrix, width - LL + 1, height))
		{
			return MEMORY_FAIL;
		}

		if(FALSE == make2DUINTarray(&AccCostMatrix, width - LL + 1, height))
		{
			return MEMORY_FAIL;
		}

		CutPos = (int *)calloc(height - 0, sizeof(int));
		if(CutPos == NULL)	//add.bong.2010-11-25 10:27AM
		{
			DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
			return MEMORY_FAIL;
		}

		for(vert = 0; vert < height; vert++)
		{
			for(horz = LL; horz < width; horz++)
			{
				CostMatrix.data[(vert - 0)][horz - LL] = myabs((int)imgL_tmp_data[vert][horz - LL] - (int)imgL_data[vert][horz]);

				//DBG_PRINTF_SDH_DEBUG("%f\n", a);
			}
		}

		boundary = (int *)calloc(height, sizeof(int));
		if(boundary == NULL)	//add.bong.2010-11-25 10:27AM
		{
			DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, height);
			return MEMORY_FAIL;
		}

		overlap_boundary(imgL, boundary, width, height, LL);
		getStitchingCut(&CostMatrix, &AccCostMatrix, CutPos, CostMatrix.width, CostMatrix.height);
		free(CostMatrix.data[0]);
		free(CostMatrix.data);
		free(AccCostMatrix.data[0]);
		free(AccCostMatrix.data);

		blend_w = width - LL;

		if(FALSE == make2Dfloatarray(&gaussian_weight, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		if(FALSE == make2Dfloatarray(&gaussian_weight_tmp, blend_w, height))
		{
			return MEMORY_FAIL;
		}

		setZerofloat(&gaussian_weight_tmp);

		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz <= max(CutPos[vert], boundary[vert]); horz++)
			{
				gaussian_weight_tmp.data[vert][horz] = 1.0f;
			}
		}

		free(CutPos);
		free(boundary);

		gaussian(&gaussian_weight_tmp, &gaussian_weight, 5.0f);
		deletefloat2D(&gaussian_weight_tmp);

		for(vert = 0; vert < height; vert++)
		{
			for(horz = 0; horz < blend_w; horz++)
			{
				imgL_data[vert][horz + LL] = (unsigned char)((float)imgL_tmp_data[vert][horz] * gaussian_weight.data[vert][horz] +
					(float)imgL_data[vert][horz + LL] * (1.0f - gaussian_weight.data[vert][horz]));
			}
		}

		deleteBYTE2D(imgL_tmp);
		deletefloat2D(&gaussian_weight);
		break;
#endif
	}

	return FALSE;
}

int blendingColor(BYTE2Dcolor *imgL, BYTE2Dcolor *imgL_tmp, int width, int height, int LL, int LL2, int blendtype) //void->int. mod.bong.2010-11-25 10:34AM
{
	int horz, vert;
	float *weight;
	int blend_w;
	int LL3;
	RgbPixel **imgL_data = imgL->data;
	RgbPixel **imgL_tmp_data = imgL_tmp->data;

	// Linear blending (blend type = 1)
	blend_w = width - LL2;
	weight = (float *)calloc(blend_w, sizeof(float));
	if(weight == NULL)	//add.bong.2010-11-25 10:27AM
	{
		DBG_PRINTF_SDH_ERR("ERROR[%d]:calloc[%d]\n", __LINE__, blend_w);
		return MEMORY_FAIL;
	}

	for(horz = 0; horz < blend_w; horz++)
	{
		weight[horz] = (float)horz / (float)(blend_w - 1);
	}

	LL3 = LL2 - LL;

	//DBG_PRINTF_SDH_DEBUG("%d\n", LL3);
	for(vert = 0; vert < height; vert++)
	{
		for(horz = 0; horz < LL3; horz++)
		{
			imgL_data[vert][horz + LL].r = imgL_tmp_data[vert][horz].r;
			imgL_data[vert][horz + LL].g = imgL_tmp_data[vert][horz].g;
			imgL_data[vert][horz + LL].b = imgL_tmp_data[vert][horz].b;
		}
	}

	for(vert = 0; vert < height; vert++)
	{
		for(horz = 0; horz < blend_w; horz++)
		{
			imgL_data[vert][horz + LL2].r = (unsigned char)(((float)imgL_tmp_data[vert][horz + LL3].r * (1.0f - weight[horz])) +
				(float)imgL_data[vert][horz + LL2].r * weight[horz] + 0.5f);
			imgL_data[vert][horz + LL2].g = (unsigned char)(((float)imgL_tmp_data[vert][horz + LL3].g * (1.0f - weight[horz])) +
				(float)imgL_data[vert][horz + LL2].g * weight[horz] + 0.5f);
			imgL_data[vert][horz + LL2].b = (unsigned char)(((float)imgL_tmp_data[vert][horz + LL3].b * (1.0f - weight[horz])) +
				(float)imgL_data[vert][horz + LL2].b * weight[horz] + 0.5f);
			//imgL_data[vert][horz + LL2] = (unsigned char)((float)imgL_tmp_data[vert][horz + LL3] + ((float)imgL_data[vert][horz + LL2] - (float)imgL_tmp_data[vert][horz + LL3]) * weight[horz] + 0.5f);
			//(unsigned char)(((float)imgL_tmp_data[vert][horz + LL3] * (1.0f - weight[horz])) + (float)imgL_data[vert][horz + LL2] * weight[horz] + 0.5f);	
		}
	}

	deletecolor2D(imgL_tmp);
	free(weight);

	return FALSE;
}

void dumpimage(BYTE2Dgray *img, char *fname)
{
	UINT32 width = img->width;
	UINT32 height = img->height;

	int c, i, j;

	FILE *ifp;
	if((ifp = fopen(fname, "wb")) == NULL)
	{
		return;
	}
	else
	{
		for(i = 0; i < height; i++)
		{
			for(j = 0; j < width; j++)
			{
				c = putc(img->data[i][j], ifp);
				if(c == EOF)
				{
					DBG_PRINTF_SDH_DEBUG("Reach to End Of File\n");
					break;
				}
			}
		}
	}

	fclose(ifp);
}

void BorderMargin(BYTE2Dgray *img, int cut_px, unsigned char side)
{
	int w, h;

	for(w = 0; w < img->width; w++)
	{
		if (side & BODDER_SIDE_TOP)
		{
			for(h = 0; h < cut_px; h++)
				img->data[h][w] = WHITE_VALUE;
		}
		if (side & BODDER_SIDE_BOTTOM)
		{
			for(h = 0; h < cut_px * 2; h++)
				img->data[img->height - 1 - h][w] = WHITE_VALUE;
		}
	}

	for(h = 0; h < img->height; h++)
	{
		for(w = 0; w < cut_px; w++)
		{
			if (side & BODDER_SIDE_LEFT)
				img->data[h][w] = WHITE_VALUE;
			if (side & BODDER_SIDE_RIGHT)
				img->data[h][img->width - 1 - w] = WHITE_VALUE;
		}
	}
}

void BorderMarginColor(BYTE2Dcolor *img, int cut_px)
{
	int w, h;

	for(w = 0; w < img->width; w++)
	{
		for(h = 0; h < cut_px; h++)
		{
			img->data[h][w].r = WHITE_VALUE;
			img->data[h][w].g = WHITE_VALUE;
			img->data[h][w].b = WHITE_VALUE;
		}

		for(h = 0; h < cut_px * 2; h++)
		{
			img->data[img->height - 1 - h][w].r = WHITE_VALUE;
			img->data[img->height - 1 - h][w].g = WHITE_VALUE;
			img->data[img->height - 1 - h][w].b = WHITE_VALUE;
		}
	}

	for(h = 0; h < img->height; h++)
	{
		for(w = 0; w < cut_px; w++)
		{
			img->data[h][w].r = WHITE_VALUE;
			img->data[h][w].g = WHITE_VALUE;
			img->data[h][w].b = WHITE_VALUE;
			img->data[h][img->width - 1 - w].r = WHITE_VALUE;
			img->data[h][img->width - 1 - w].g = WHITE_VALUE;
			img->data[h][img->width - 1 - w].b = WHITE_VALUE;
		}
	}
}

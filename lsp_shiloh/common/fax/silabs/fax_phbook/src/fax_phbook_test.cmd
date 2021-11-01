pbset 1 1-111-111-1111 "Company One" 0
pbset 8 888-8888 "Crazy Eights" 0
pbset 12 222-2222 "Grandma & Grandpa" 0
pbset 13 3-333-333-333 "Cousin Trinity" 0
pbset 14 4-444-444-444 "Aunt April" 0
pbset 15 555-5555 "Uncle Eddie" 0
pbset 50 123-4567 "Collection Agency" 1
pbset 51 555-9876 "Phoney Phisher" 1
pbgrpclr 0 1 2
pbgrpnm 0 Family
pbgrpadd 0 12 13 14 15
pbgrpnm 1 Businesses
pbgrpadd 1 1 8
pbgrpnm 2 "Darn Spammers"
pbgrpadd 2 50 51
pbdump
puts "So Sad - Grandma & Grandpa Died"
pbclr 12
pbdump
puts "Add a new annoying caller to my blacklist"
pbset 52 1-800-84674273 "1 800 Timeshares" 1
pbgrpadd 2 52
pbdump
puts "Clean up everybody but family"
pbclr 1 8 50 51 52
pbdump
puts "Trinity got married - add husband's last name"
pbset 13 3-333-333-333 "Cousin Trinity Triad" 0
pbdump

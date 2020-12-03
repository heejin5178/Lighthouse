# ========== make cscope file list ==========
# do below
# sudo chmod 755 mkcscope.sh
# sudo mv mkcscope.sh /usr/local/bin

#!/bin/sh
rm -rf cscope.files cscope.files

find . \( -name '*.c' -o -name '*.cpp' -o -name '*.cc' -o -name '*.h' \
	-o -name '*.s' -o -name '*.S' -o -name '*.asm' \) -print > cscope.files

cscope -i cscope.files

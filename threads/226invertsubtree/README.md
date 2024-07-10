```bash
 g++ -std=c++11 -fsanitize=thread -g -o wrong-invertsubtree wrong-invertsubtree.cpp
./wrong-invertsubtree 

 g++ -std=c++11 -fsanitize=thread -g -o invertsubtree invertsubtree.cpp
./invertsubtree
```    


dSYM 文件夹包含调试符号文件（debug symbols），这些文件是编译器生成的，用于帮助调试程序。当你在编译程序时启用了调试选项（例如使用 -g 选项），编译器会生成调试信息，并存储在 .dSYM 文件夹中。这些信息包括变量名、函数名、行号等，有助于调试器（例如 lldb 或 gdb）在程序崩溃或遇到断点时显示有用的调试信息。

具体来说：

Info.plist：包含有关调试符号文件的元数据信息。
Resources：
DWARF：包含实际的调试符号文件，这些文件使用 DWARF 格式存储调试信息。
Relocations：包含重定位信息（用于链接器在运行时正确加载调试信息）。
例如：

invertsubtree.dSYM 文件夹中的 DWARF/invertsubtree 文件包含了 invertsubtree.cpp 编译生成的调试符号。
wrong-invertsubtree.dSYM 文件夹中的 DWARF/wrong-invertsubtree 文件包含了 wrong-invertsubtree.cpp 编译生成的调试符号。
这些调试符号文件对于调试器来说非常重要，因为它们可以帮助你在源代码级别调试程序，而不仅仅是看到机器代码。
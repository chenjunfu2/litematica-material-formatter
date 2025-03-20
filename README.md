# litematica-material-formatter 介绍
Litematica材料列表转换器，将Litematica输出的material_list转换到更易读的形式并尽可能对齐输出

带old后缀的是老版本代码，另一个是新版本代码

新版几乎完全重构了老版本的垃圾代码，虽然写的还是很垃圾，但是至少好一点（）

主打一个能用就行，比老版本多了个批量文件转换，和文件覆盖提示

**PS：因为本人技术原因（太菜），导致文本支持只支持本地ANSI编码，暂时不支持utf-8编码，
如果直接拖拽转换后输出文件内出现乱码，请用记事本打开原材料列表文件->另存为->编码为ANSI->保存**

# 新版用法
拖拽Litematica输出的material_list_xxx.txt文件（可以同时选择多个）到程序上松开即可自动转换，输出文件与输入文件在相同目录下。

如果目录下已有相同输出文件则会要求确认是否覆盖，y就是确认覆盖当前处理的文件，n就是不覆盖跳过当前处理的文件，a就是覆盖全部文件。

转换示例（程序输出左 litematica导出右）：
![转换示例](https://github.com/chenjunfu2/litematica-material-formatter/blob/main/%E8%BD%AC%E6%8D%A2%E7%A4%BA%E4%BE%8B.png)

覆盖提示：
![覆盖提示](https://github.com/chenjunfu2/litematica-material-formatter/blob/main/%E8%A6%86%E7%9B%96%E6%8F%90%E7%A4%BA.png)

# 旧版用法
相比新版只能拖拽一个，输出没有对齐，覆盖无提示。

与新版对比（旧版左 新版右）：
![对比输出](https://github.com/chenjunfu2/litematica-material-formatter/blob/main/%E6%97%A7%E7%89%88%E8%BE%93%E5%87%BA%E4%B8%8E%E6%96%B0%E7%89%88%E8%BE%93%E5%87%BA.png)


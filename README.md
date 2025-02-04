# litematica-material-formatter
Litematica材料列表转换器，将Litematica输出的material_list转换到更易读的形式并尽可能对齐输出

带old后缀的是老版本代码

带new后缀的是新版本代码

新版几乎完全重构了老版本的垃圾代码，虽然写的还是很垃圾，但是至少好一点（）

主打一个能用就行，比老版本多了个批量文件转换，和文件覆盖提示

![转换示例](https://github.com/chenjunfu2/litematica-material-formatter/blob/main/%E8%BD%AC%E6%8D%A2%E7%A4%BA%E4%BE%8B.png)

# 新版用法
拖拽Litematica输出的material_list_xxx.txt文件（可以同时选择多个）到程序上松开即可自动转换，

如果已有相同输出文件则会要求确认是否覆盖，y就是确认覆盖当前处理的文件，n就是不覆盖跳过当前处理的文件，a就是覆盖全部文件。

# 老版用法
相比新版只能拖拽一个，输出没有对齐，覆盖无提示。

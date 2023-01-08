第一步（获取数据集）              解压文件夹'Dataset'中的压缩包'LFSD.zip'以及'NJU2K.zip'获得数据集。
第二步（计算深度显著图）          打开文件夹'1_acsdSaliency'中的文件'acsdSaliency.sln'，修改'demo.cpp'中填写需要处理的数据集的名称（LFSD或NJU2K），然后进行编译。
第三步（融合深度显著图）          打开文件夹'2_Pre-Processing'中的文件'Process_org_image.m',修改数据集的名称（LFSD或NJU2K），然后进行编译。
第四步（多任务FCN计算初始显著图） 在Ubuntu系统下（配置好Caffe环境），在文件夹'3_deepSaliency'下打开终端，修改文件'demo.py'中待处理数据集的名称（LFSD或NJU2K）,并在Caffe环境下执行'python demo.py'。
第五步（细化显著图）              打开文件夹'4_Refinement'中的文件'Runme_forBLsaliencymap.m',修改数据集的名称（LFSD或NJU2K），然后进行编译，最终的显著图存放在'../SaliencyMap/LFSD/Ours'和'../SaliencyMap/NJU2K/Ours'下。
第六步（客观评价）                打开文件夹'../5_Evaluation/tools/'中的文件'evaluate_models.m',运行程序，选择要评价的模型以及数据集，其结果存放于'../5_Evaluation/Results/'下。
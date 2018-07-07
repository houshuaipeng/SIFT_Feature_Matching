# SIFT_Feature_Matching

*****问题：OpenCV中关于SIFT特征匹配的代码有一个缺点，就是当图片大小不一致时，匹配的结果也是不同的，比如说：
当小图在左边，大图在右边的时候，匹配结果正确，但是当大图在左边，小图在右边的时候，就会出现很多误匹配的点
*****原因：小图在左边，大图在右边时，相当于拿较少的特征点与较多的特征点进行匹配，得到的匹配数量最多为小图的特征点的数量；
但是当拿较多的特征点与较少的特征点进行匹配时，得到的匹配数量最多为大图的特征点的数量，因为大图中可能多个特征点与小图
中同一个特征点匹配上，从而造成两种情况下匹配结果的不同。
*****解决方法：添加用于交叉验证的函数void symmetryTest()，该算法进行两次匹配，第一次匹配可以使用前两种的匹配算法，
第二次匹配时，使用的匹配算法的执行顺序与第一次匹配的顺序相反，
 * 将第二幅图像的每个关键点逐个与第一幅图像的全部关
键点进行比较。
 * 只有两个方向上都匹配到了同一对特征点，才认为是一个有效的匹配对。
void symmetryTest()//该函数用于交叉验证
vector<DMatch> chooseGood()//该函数用于挑选较好的匹配对（只有距离小于最小距离的固定倍数的匹配的对才被选中）
main函数流程：加载图片、显示原图、SIFT特征检测、显示带有特征点的图片、在特征点上生成特征描述符、生成匹配对、优选、交叉验证、显示结果
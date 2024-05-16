#set text(
  font: ("linux libertine", "Source Han Serif SC", "Source Han Serif"),
  size: 10pt,
)
#set heading(numbering: "1.")

#set page(footer: context [
  #if counter(page).get().first() > 1 [
    *USTC/ALGO24 算法基础 大作业报告*
  ]
  #h(1fr)
  #counter(page).display(
    "1/1",
    both: true,
  )
])

#box(width: 100%, height: 30%)[
  #align(right + horizon)[
    #text(weight: "bold", size: 15pt)[
      USTC/ALGO24 算法基础

      GraphCut 大作业报告
    ]

    马天开 PB21000030
  ]
]

= GrabCut

#text(style: "italic")[
  Disclaimer:
  - Reimplmentation of paper: #link("https://doi.org/10.1145/1186562.1015720")

    Carsten Rother, Vladimir Kolmogorov, and Andrew Blake. 2004. "GrabCut": interactive foreground extraction using iterated graph cuts. In ACM SIGGRAPH 2004 Papers (SIGGRAPH '04).

  - Some part of OpenCV implementation is used.
]

== 算法思路

+ 将图像视作一个图 _(graph)_

  + 每个像素视作一个节点 _(node)_

  + 相邻 (有共同邻边, 即 $4-$ 相邻) 像素之间连有边, 记作 `n-link`

  + 引入 `S` 节点和 `T` 节点, 分别代表前景 _(foreground)_ 和背景 _(background)_, 所有节点和这两个节点相连, 记作 `t-link` (`s-t-link` & `t-t-link`)

+ 各条边的#highlight[权重] _(weight)_:

  + 图片的前景/背景的内部, 像素之间的差异较小; 前景与背景连接的部分, 像素之间的差异较大.

    $=>$ 转化为最小割问题, 边的权重应该定义为一个跟像素之间的差异负相关的函数.

  + 通过标记数据, 一些区域我们已经预先地知道是前景/背景

    $=>$ 转化为最小割问题, 例如, 如果我们已知某个像素是前景, 那么这个像素只与 `S` 连接. (也可以与 `T` 连接一个权重为 $0$ 的边)

  + 未明确标记的区域, 我们可以通过后验概率来计算像素最有可能在前景/背景的概率, 从而计算权重.

    $=>$ 转化为最小割问题, 如果像素更有可能是前景, 那么与 `S` 相连的权重更大.

+ 在这个图上运行#highlight[最小割算法], 得到分割结果

  $=>$ 分割结束后, 与 `S` 相连的节点视作前景, 与 `T` 相连的节点视作背景.

== 实现细节
=== 数学建模

$
  &G=angle.l V, E angle.r\
  &V = cal(P) union {S, T}, quad &&cal(P) = {P_1, ..., P_(abs(cal(P)))}\
  &E = E_1 union E_2 union E_3, quad && E_1 = {(P_i, P_j) | 1 <= i, j <= abs(cal(P))}\
  &("source")&& E_2 = {(S, P_i) | 1 <= i <= abs(cal(P))}\
  &("sink")&& E_3 = {(P_i, T) | 1 <= i <= abs(cal(P))}
$

=== 损失函数

对分割评估的质量的评估, 也就是对边权重的评估. 一般来说, 考虑采用如下方式表示分割结果:
$
  A = {A_1, ..., A_(abs(cal(P)))}
$

其中
$
  A_i = cases(0 quad A_i "linked to" T, 1 quad A_i "linked to" S)
$

对分割结果 $A$ 的评估分为两部分:

$
  E(A) = lambda R(A) + B(A) quad lambda > 0
$

在这其中:
- $R(A)$: Region properties, 衡量 `t-link` 被切割的代价.
$
  R(A) = sum_(p in cal(P)) R_p(A_p)
$
- $B(A)$: Boundary properties, 衡量 `n_-link` 被切割的代价.
$
  B(A) = sum_((p,q) in E_1) B(p, q) dot delta(A_p, A_q)
$

==== `t-link` 的损失函数

`t-link` 的损失函数主要依赖于后验概率 $PP(dot mid(|) cal(F)), PP(dot mid(|) cal(B))$, 通过这样的后验概率分布, 我们可以使用如下的方式计算:

$
  cases(
PP(I_p mid(|) cal(F)) = PP(I_p and cal(F)) \/ PP(cal(F)),
PP(I_p mid(|) cal(B)) = PP(I_p and cal(B)) \/ PP(cal(B))
) => cases(
R_p (A_p = 1) = - ln PP(I_p mid(|) cal(F)),
R_p (A_p = 0) = - ln PP(I_p mid(|) cal(B))
)
$

==== `n-link` 的损失函数

$
  B(p, q) prop exp(- (I_p - I_q)^2 / (2 sigma^2)) 1 / ("dist"(p, q))
$

== Notes

=== OpenCV

OpenCV 的坐标系统有些混乱:

+ 一般来说, OpenCV 坐标系是以左上角为原点, 向右为 x 轴正方向, 向下为 y 轴正方向. 这和我们一般的坐标系是不同的.

+ 在输出矩阵的时候, 坐标系是以左上角为原点, 向下为 x 轴正方向, 向右为 y 轴正方向. 这跟矩阵的定义是类似的.

=== `mask.png`

GrabCut 实际上允许我们在 mask 中指定前景 & 背景, 这里出于 demo 方便的用途, 所有非黑色的部分都会被视作「确定的背景」.

==== Build

注意: 需要安装 OpenCV 并配置好 CMake 等工具链.

我们使用 CMake 来构建项目, 项目已经配置 CMake 拷贝 `input.png` `mask.png` 到 binary 目录下 (`./build`), 此时 `output.png` 等也生成在 binary 目录下.

=== Run

- 测试用例: `./grabcut`

- 完整参数: `./grabcut input.png mask.png min_x min_y max_x max_y output.png`

=== 可能的改进

- 完善 `mask.png` 的标记 (前景/背景)
- 交互式标记
- 优化参数 ($gamma, lambda, beta$) 等

== 运行结果

随代码打包了测试用例 `input.png` `mask.png`, 框选矩形的参数写在 `grabcut.cpp` 中 (默认参数), 运行结果如下:


#table(
  columns: (1fr, 1fr, 1fr),
  stroke: none,
  align: center,
  gutter: 0em,
  table.cell(colspan: 3)[
    #align(left)[
      #rect(fill: red, width: 100%)[
        #text(fill: white)[
          输入 / Input
        ]
      ]
    ]
  ],
  [
    #image("./img/input.png")
  ],
  [
    #image("./img/input-framed.png")
  ],
  [
    #image("./img/mask.png")
  ],
  table.cell(colspan: 3)[
    #align(left)[
      #rect(fill: blue, width: 100%)[
        #text(fill: white)[
          输出 / Output
        ]
      ]
    ]
  ],
  [
    #image("./img/output-mask-init.png")
  ],
  [
    #image("./img/output-mask.png")
  ],
  [
    #image("./img/output.png")

  ],
)

#pagebreak(weak: true)

= GraphCut

#text(style: "italic")[
  Disclaimer:
  - Reimplmentation of paper: #link("https://sites.cc.gatech.edu/cpl/projects/graphcuttextures/gc-final-lowres.pdf")

    Vivek Kwatra and Arno Schödl and Irfan Essa and Greg Turk and Aaron Bobick. Graphcut Textures: Image and Video Synthesis Using Graph Cuts. In ACM SIGGRAPH 2003 Papers (SIGGRAPH '03).
]

== 算法思路

依旧是将图像的问题转化为一张图, 与上面的 GrabCut 不同在于, 这里需要多次将一张图像叠加到结果上, 因此需要解答下面的问题:

- 图的构建:

  考虑基础情况: 两张图片 $A, B$, 定义重合部分 $"overlap"(A,B)$, 目标是寻求某种分割方式, 使得重合区域的过渡更加自然. 将每个像素点视为一个 node, 相邻像素之间连接一条 `n-link`, 额外定义两个节点 `S` 和 `T`, 分别与 $A$ 和 $B$ 的重合部分的*边缘*相连, weight 定义为 $+oo$

- 像素之间的权重计算方式:

  既然是将新的图像叠加到结果上, 对于重叠部分的像素, 两个节点 $s$ 和 $t$, 以及他们在原始图像的颜色 $A(s)$ 和在新 patch 上的颜色 $B(t)$, 我们需要定义一个损失函数 $M(s, t, A, B)$:

$
  M(s, t, A, B) = norm(A(s)-B(s)) + norm(A(t)-B(t))
$

- 偏移位置选取:

  需要思考新的 patch 摆放的位置. 可以考虑以下方式进行:

  + Random

    标记未覆盖的像素点, 在一个未覆盖的像素点附近随机选取位置, 保证新的 patch 一定能覆盖这个像素点.

  + Entire Match

    将上面的 Random 的范围拓展到全局, 在此基础上按这样的概率密度取得偏移量 $t$:
    $
      C(t) &= 1 / abs(A_t) sum_(p in A_t) norm(I(p)-O_t (p))^2 quad A_t = "overlap"(I, O_t)\
      p & prop exp(-C(t)\/k sigma^2)
    $

    其中 $sigma$ 为输入图片的标准差, $k$ 为超参数, 取 $k=0.01$. 为保证效率, 同样只考虑能覆盖新像素的偏移.

  + Subpatch Match

    跟上面的方法类似, 在一生成的图片中选取某个完全被覆盖的 sub patch, 记为 $S_0$, 针对这个区域计算损失即可:

    $
      C(t)=sum_(p in S_0) norm(I_(-t)(p)-O(p))^2
    $

== 实现细节

=== 基于梯度的损失函数

为让算法更倾向于风格梯度变化更大的区域, 在上面提到的损失函数上增加像素点附近的梯度信息:
$
  M'(s,t,A,B)=(M(s,t,A,B)) / (norm(G'_A (s))+norm(G'_B (s))+norm(G'_A (t))+norm(G'_B (t)))
$

其中 $s,t$ 为像素点, 这里的导数沿 $s-t$ 的方向.

=== 重合部分对 old seam 的重新评估

在上述过程中, 对已经生成的部分都是当作*一张*图片进行处理的, 可以考虑如下方法对已经合并的部分进行重新评估. 引入新节点 _seam_node_, 依旧采用最小割的方法判断 old seam 是否应该被 new patch 覆盖, 实现细节如下:

$
  s(s,t) -> B quad "weight"=M(s,t,A,B)\
  s -> s(s,t) quad "weight"=M(s,t, A_s, B)\
  s -> t(s,t) quad "weight"=M(s,t, B, A_t)
$

=== FFT optimization

对于偏移位置选取的后两种算法, 使用 FFT 可对其计算进行加速:

$
  C(t) = sum_p I_(-t)^2(p)^2 + sum_p O^2(p) - sum_p 2 I_(-t)(p) O(p)
$

前两部分的计算中, 如果不考虑填充完全的问题, 可以使用二维前缀和进行计算, 转移方程:

$
  D(i,j) = D(i-1,j) + D(i,j-1) - D(i-1,j-1) + I(i,j)^2
$

对于任意的矩形区域: $[a,b,c,d]$, 其内部的平方和可以利用下面的方法计算:

$
  sum = D[c,d] - D[a-1,d] - D[c,b-1] + D[a-1,b-1]
$

上述方法复杂度: $O(n)$. 通过将上述问题转化为类似卷积的方式进行计算, 需要额外添加 $"Bitmap"(O)$ 来代表 mask, 利用其对图片平方进行卷积即可. 使用 FFT 后, 整体时间复杂度 $O(n lg n)$

=== Surrounded Region

考虑 new patch 位置选取在已经被完全覆盖的区域, 如果不考虑 old seam 修正的方法, 这个图中将不再出现 sink, 因为重合区域没有和 new patch 相邻的像素, 可以特殊处理这样的问题, 强制部分像素与 sink 相连即可.

针对 old seam 修正后不会出现类似的问题, 因为 seam node 总是和 new patch 对应的 sink 相邻, 无需上述操作.

== 运行结果

#table(
  columns: (1fr, 2fr),
  stroke: none,
  align: center,
  gutter: 0em,
  [
    #rect(fill: red, width: 100%)[
      #align(left)[
        #text(fill: white)[
          输入 / Input
        ]
      ]
    ]
  ],
  [
    #rect(fill: blue, width: 80%)[
      #align(left)[
        #text(fill: white)[
          输出 / Output
        ]
      ]
    ]
  ],

  [
    #image("img/strawberry.png")
  ],
  [
    #scale(90%, reflow: true)[
      #image("img/strawberry_new.png")
    ]
  ],
)

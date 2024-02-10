# Ray Tracing: the Rest of Your Life

<a href="https://raytracing.github.io" style="font-size:20px;font-weight:bolder">系列网址</a>        <a href="https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html" style="font-size:20px;font-weight:bolder">本书网址</a>

------



### 构建

`git clone` 之后，进入文件夹中，在 `cmd` 中执行：

```cmd
mkdir build
cd build
cmake ..
```

默认 `C++` 标准是 **17**，可以在 `CMakeLists.txt` 中修改。

`cmake` 默认使用 `MSVC` 构建项目，将最后一句换成 `cmake -G "MinGW Makefiles" ..` 可以使用 `MinGW` 构建项目。不过，经实践表明，使用 `MinGW` 构建的项目的渲染速度明显慢于 `MSVC` ，原因我不知道。



---



### 编译链接

**MSVC:**

```cmd
cmake --build . --config Release
```

或者打开 `build/` 下的 `.sln` ，在 `Visual Studio` 中编译。

**MinGW:**

```cmd
cmake --build .
```



---



### 渲染

将装载好的程序重定向到 `.ppm` 文件中，在 `cmd` 中执行：

```cmd
build\Release\RayTracing.exe > image.ppm
```

⚠️ 经本人试验，使用 `PowerShell` 重定向产生的 `.ppm` 无法查看，原因不详。



---



### 查看PPM图片

可以使用各种功能丰富的图片查看器查看，请自行了解。

我用的是VSCode的插件  ***PBM/PPM/PGM Viewer for Visual Studio Code***。

想要查看图片的话需要在文件夹中打开`.ppm`，如下所示：

<img src="https://telegraph-image-avq.pages.dev/file/e96ca8705ee0b7b08f927.png" style="zoom: 33%;" />

使用记事本或者直接打开 `.ppm` 可以看到源数据：

<div><span><img src="https://telegraph-image-avq.pages.dev/file/219d6b7c0ecfba117b764.png" alt="记事本打开" style="zoom: 40%;" /></span><span><img src="https://telegraph-image-avq.pages.dev/file/2bf65b16ff78112875c13.png" style="zoom:30%;" /></span></div>
















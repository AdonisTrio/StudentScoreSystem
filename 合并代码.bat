@echo off
chcp 65001 >nul
del 全部代码.txt 2>nul

for /r %%f in (*.h *.cpp) do (
    echo ============================================== >> 全部代码.txt
    echo 文件名：%%f >> 全部代码.txt
    echo ============================================== >> 全部代码.txt
    type "%%f" >> 全部代码.txt
    echo. >> 全部代码.txt
    echo. >> 全部代码.txt
)

echo 合并完成！
pause
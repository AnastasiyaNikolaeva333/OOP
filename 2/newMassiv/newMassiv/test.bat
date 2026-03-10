@echo off 
chcp 65001 > nul

set "PROGRAM=%~1"
set OUT="%TEMP%\out.txt"

echo 1 Test
%PROGRAM% < data_test/test1.txt > %OUT% || goto err
fc %OUT% data_test/test1-out.txt || goto err

echo 2 Test
%PROGRAM% < data_test/test2.txt > %OUT% || goto err
fc %OUT% data_test/test2-out.txt || goto err

echo 3 Test
%PROGRAM% < data_test/test3.txt > %OUT% || goto err
fc %OUT% data_test/test3-out.txt || goto err

echo 4 Test(ERROR)
%PROGRAM% < data_test/test4.txt > %OUT% || goto err
fc %OUT% data_test/test4-out.txt || goto err

echo 5 Test
%PROGRAM% < data_test/test5.txt > %OUT% || goto err
fc %OUT% data_test/test5-out.txt || goto err

::max
echo 6 Test(ERROR)
%PROGRAM% < data_test/test6.txt > %OUT% || goto err
fc %OUT% data_test/test4-out.txt || goto err

echo 7 Test(ERROR)
%PROGRAM% < data_test/test7.txt > %OUT% || goto err
fc %OUT% data_test/test4-out.txt || goto err


echo All tests passed
exit /B 0

:err
echo Program testing failed
exit /B 0
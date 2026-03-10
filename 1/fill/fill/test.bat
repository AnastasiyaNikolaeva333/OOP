@echo off 
chcp 65001 > nul

set "PROGRAM=%~1"
set OUT="%TEMP%\out.txt"


echo 2 Test
%PROGRAM% data_test/test1.txt output.txt> %OUT% || goto err
fc output.txt data_test/test1-out.txt || goto err

echo 3 Test
%PROGRAM% < data_test/test1.txt > %OUT% || goto err
fc %OUT% data_test/test1-out.txt || goto err

echo 4 Test
%PROGRAM% data_test/test2.txt output.txt> %OUT% || goto err
fc output.txt data_test/test2-out.txt || goto err

echo 5 Test
%PROGRAM% < data_test/test2.txt > %OUT% || goto err
fc %OUT% data_test/test2-out.txt || goto err

echo 6 Test
%PROGRAM% data_test/test3.txt output.txt> %OUT% || goto err
fc output.txt data_test/test3-out.txt || goto err

echo 7 Test
%PROGRAM% < data_test/test3.txt > %OUT% || goto err
fc %OUT% data_test/test3-out.txt || goto err

echo 8 Test(spill)
%PROGRAM% data_test/test4.txt output.txt> %OUT% || goto err
fc output.txt data_test/test4-out.txt || goto err

echo 9 Test(spill)
%PROGRAM% < data_test/test4.txt > %OUT% || goto err
fc %OUT% data_test/test4-out.txt || goto err

echo 10 Test(spill)
%PROGRAM% < data_test/test7.txt > %OUT% || goto err
fc %OUT% data_test/test5-out.txt || goto err

echo 10 Test(split)
%PROGRAM% < data_test/test6.txt > %OUT% || goto err
fc %OUT% data_test/test6-out.txt || goto err

echo 11 Test(ERROR -)
%PROGRAM% < data_test/test5.txt > %OUT% || goto err
fc %OUT% data_test/test7-out.txt || goto err

echo 12 Test(ERROR input123.txt)
%PROGRAM%  input123.txt  output.txt > %OUT% 
fc %OUT% data_test/test7-out.txt || goto err

echo 12 Test(ERROR 1 arg)
%PROGRAM%  input123.txt   > %OUT% 
fc %OUT% data_test/test7-out.txt || goto err

echo 13 Test(ERROR 1 arg = 2 arg)
%PROGRAM%  input123.txt  input123.txt > %OUT% 
fc %OUT% data_test/test7-out.txt || goto err

echo All tests passed
exit /B 0

:err
echo Program testing failed
exit /B 1
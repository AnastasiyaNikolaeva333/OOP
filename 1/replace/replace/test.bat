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

echo 4 Test
%PROGRAM% < data_test/test4.txt > %OUT% || goto err
fc %OUT% data_test/test4-out.txt || goto err

echo 5 Test
%PROGRAM% < data_test/test5.txt > %OUT% || goto err
fc %OUT% data_test/test5-out.txt || goto err

echo 6 Test
%PROGRAM% < data_test/test6.txt > %OUT% || goto err
fc %OUT% data_test/test6-out.txt || goto err

echo 7 Test
%PROGRAM% data_test/test7.txt output.txt ma mama > %OUT% || goto err
fc output.txt data_test\test1-out.txt || goto err

echo 8 Test
%PROGRAM% data_test/test8.txt output.txt ma mama > %OUT% || goto err
fc output.txt data_test\test2-out.txt || goto err

echo 9 Test
%PROGRAM% data_test/test9.txt output.txt 1231234 XYZ > %OUT% || goto err
fc output.txt data_test\test3-out.txt || goto err

echo 10 Test
%PROGRAM% output.txt output.txt 1231234 XYZ > %OUT% 
fc %OUT% data_test\test6-out.txt || goto err

echo All tests passed
exit /B 0

:err
echo Program testing failed
exit /B 1
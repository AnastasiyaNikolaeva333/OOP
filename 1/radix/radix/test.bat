@echo off
chcp 65001 > nul
::TODO 01 в числе иной символ, нереальное число

set "PROGRAM=%~1"
set OUT="%TEMP%\output.txt"

echo 1 Test: 16 10 1F
%PROGRAM% 16 010 1F > %OUT% || goto err
fc %OUT% data_test\test1-out.txt || goto err

echo 2 Test: 10 37 31 (ERROR)
%PROGRAM% 10 37 31 > %OUT% || goto err
fc %OUT% data_test\test2-out.txt || goto err

echo 3 Test: 2 10 1010
%PROGRAM% 2 10 1010 > %OUT% || goto err
fc %OUT% data_test\test3-out.txt || goto err

echo 4 Test: 10 2 -42
%PROGRAM% 10 2 -42 > %OUT% || goto err
fc %OUT% data_test\test4-out.txt || goto err

echo 5 Test: 8 16 0
%PROGRAM% 8 16 0 > %OUT% || goto err
fc %OUT% data_test\test5-out.txt || goto err

echo 6 Test: 36 10 Z
%PROGRAM% 36 10 Z > %OUT% || goto err
fc %OUT% data_test\test6-out.txt || goto err

echo 7 Test: 16 10 7FFFFFFF
%PROGRAM% 16 10 7FFFFFFF > %OUT% || goto err
fc %OUT% data_test\test7-out.txt || goto err

echo 8 Test: 16 10 -80000000 
%PROGRAM% 16 10 -80000000 > %OUT% || goto err
fc %OUT% data_test\test8-out.txt || goto err

echo 9 Test: 2 10 123 (ERROR)
%PROGRAM% 2 10 123 > %OUT% || goto err
fc %OUT% data_test\test9-out.txt || goto err

echo 10 Test: 1 10 (ERROR)
%PROGRAM% 1 10 > %OUT% || goto err
fc %OUT% data_test\test10-out.txt || goto err

echo 11 Test: 8 o 7 (ERROR)
%PROGRAM% 8 o 7 > %OUT% || goto err
fc %OUT% data_test\test11-out.txt || goto err

echo 12 Test: 016 010 01F
%PROGRAM% 016 010 01F > %OUT% || goto err
fc %OUT% data_test\test1-out.txt || goto err

echo 13 Test: 16 10 1F00000000000000000000000
%PROGRAM% 16 10 1F00000000000000000000000000 > %OUT% || goto err
fc %OUT% data_test\test9-out.txt || goto err

echo 14 Test: 16 10 1F
%PROGRAM% 1600000000 10 1F > %OUT% || goto err
fc %OUT% data_test\test9-out.txt || goto err

echo 14 Test: 1600000000 10 1F
%PROGRAM% 1600000000 10 1F > %OUT% || goto err
fc %OUT% data_test\test9-out.txt || goto 

echo 15 Test: 16t 10 1F
%PROGRAM% 16t 10 1F > %OUT% || goto err
fc %OUT% data_test\test12-out.txt || goto err

echo 16 Test: 16 10 1F=
%PROGRAM% 16 10 1F= > %OUT% || goto err
fc %OUT% data_test\test9-out.txt || goto err

echo 17 Test: 16 10 80000000 
%PROGRAM% 16 10 80000000 > %OUT% || goto err
fc %OUT% data_test\test9-out.txt || goto err

echo 18 Test: 16 10 -80000001
%PROGRAM% 16 10 -80000001 > %OUT% || goto err
fc %OUT% data_test\test9-out.txt || goto err

echo All tests passed
exit /B 0

:err
echo Program testing failed
exit /B 1
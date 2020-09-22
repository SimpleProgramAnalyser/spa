#!/bin/bash

projectfiles=$(find ./Team12/Code12/src/spa/src -iname "*.h" -o -iname "*.cpp")
exitcode=0;
for f in $projectfiles;
do
    outputname=$(echo "$f" | rev | cut -f 1 -d '/' | rev)
    rm -f $outputname.out.temp
    clang-format-10 $f >> $outputname.out.temp
    cmp $f $outputname.out.temp
    if (( $? == 0 ));
    then
        echo "$f formatted correctly!";
        echo "";
    else
        echo $(diff $f $outputname.out.temp)
        echo "";
        exitcode=1
    fi
    rm -f $outputname.out.temp
done
exit $exitcode

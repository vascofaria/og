total=0
passed=0
perc=0

for test in ./auto-tests/*.og
do
	((total++))
	test_name=$(basename "$test")
	name=$(echo "$test_name" | cut -f 1 -d '.')

	./src/og $test -o ./auto-tests/results_asm/$name.asm
	yasm -felf32 ./auto-tests/results_asm/$name.asm -o ./auto-tests/results_asm/$name.o
	ld -melf_i386 -o ./auto-tests/results_asm/$name ./auto-tests/results_asm/$name.o -lrts
	./auto-tests/results_asm/$name > ./auto-tests/results/$name.out

	echo "=========================================$name==================================="
	diff -y -b ./auto-tests/expected/$name.out ./auto-tests/results/$name.out && echo "TEST PASSED!" && ((passed++)) || echo "TEST FAILED!"
	echo "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"

done
((perc=100 * passed / $total))

echo "RESULTS: $passed/$total - $perc %"

<<COMMENTS

outputFile="differs.txt"
echo > $outputFile

for input in ./auto-tests/expected/*.out
do
	echo "-----------------DIFFER---------------------" >> $outputFile
	mine=$(basename "$input")
	echo >> $outputFile
	echo >> $outputFile
	echo "############PROF###############" >> $outputFile
	echo >> $outputFile
	cat "$input" >> $outputFile
	echo >> $outputFile
	echo >> $outputFile
	echo "############NOSSO##############" >> $outputFile
	echo >> $outputFile
	cat "./auto-tests/results/$mine" >> $outputFile
	echo >> $outputFile
	echo >> $outputFile
	echo "__________________DIFS_____________" >> $outputFile
	echo >> $outputFile
	echo "$mine" >> $outputFile
	diff $input ./auto-tests/results/$mine >> $outputFile
	diff -B $input ./auto-tests/results/$mine && echo "TEST PASSED!" || echo "TEST FAILED!"
	echo "-----------------/DIFFER--------------------" >> $outputFile
	echo >> $outputFile
	echo >> $outputFile
done

COMMENTS
#a simple bash script that executes pmu.c with its prints saved into a output file.
#uncomment compilation if needed
#output.txt will display in terminal after generating output from pmu.c
#output.txt will be overwritten every time bash script is executed
#change "(i < 1000)" to run it as many times as you like
#your terminal might blow up idk. Have fun.
#gcc -o pmu pmu.c
for ((i=0; i < 1000; i++))
do	
	./pmu	
done > output.txt
echo "Results saved into output file, displaying output file..."
sleep 5
cat output.txt

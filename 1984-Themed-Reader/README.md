README for The Great Firewall of Santa Cruz

This program evaluates words from STDIN and evaluates
if they appear in a list of old speak or bad speak 
words. To use the program do the following:

- Use "make" or "make all" to compile the program
- Call the excecutable "newspeak" 
- Use one of the following options:
	-h   Defines size of hash table - default set to 10000
	-f   Defines size of bloom filter - default set to 2^20
	-m   Use move-to-front algorithm (Sets elements from each list to the front if they are looked up)
	-b   Not use move-to-front (Run normally)

- Type into STDIN the words that you want to check
- A message from the program will explain your word usage
- Use "make clean" to remove excecutables and object files

	* For some reason I couldn't have "make infer" work. (I will go to office hours to discuss this issue) 

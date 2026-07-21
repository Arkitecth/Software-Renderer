main:
	g++ main.cpp tgaimage.cpp -O3 -Wno-narrowing -o main

clean:
	rm -rf main

all: CacheSimulator.c
	gcc CacheSimulator.c -o CacheSimulator

github_create:
	echo "# Cache-Simulator" >> README.md
	git init
	git add README.md
	git commit -m "first commit"
	git remote add origin https://github.com/dangelox/Cache-Simulator.git
	git push -u origin master
	
github_init_push:
	git remote add origin https://github.com/dangelox/Cache-Simulator.git
	git push -u origin master
	
clean:
	rm -f *~ CacheSimulator
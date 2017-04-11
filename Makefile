all: CacheSimulator.c
	gcc CacheSimulator.c -o CacheSimulator

v1: CacheSimulator_v1.c
	gcc CacheSimulator_v1.c -o CacheSimulator_v1
	
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
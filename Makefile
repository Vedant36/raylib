
mandelbrot: mandelbrot.c
	gcc mandelbrot.c -lraylib -lm -ggdb -o mandelbrot

snake: snake.c
	gcc snake.c -lraylib -lm -ggdb -o snake

starfield: starfield.c
	gcc starfield.c -lraylib -lm -ggdb -o starfield

raytracer: raytracer.c
	gcc raytracer.c -lraylib -lm -o raytracer

brick: brick.c
	gcc brick.c -lraylib -lm -o brick

pendulum: pendulum.c
	gcc pendulum.c -ggdb -lraylib -o pendulum

doubpend: doubpend.c
	gcc doubpend.c -ggdb -lraylib -o doubpend

bird: bird.c
	gcc bird.c -ggdb -lraylib -lm -o bird

life: life.c
	gcc life.c -ggdb -lraylib -o life

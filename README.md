# Blade-lang
Blade is a joke programming language i'm making, it's based mainly on lisp and clojure, along with some inspiration taken from rpgs

# Installation
- Linux

Make sure you have __g++__ and __make__ installed and then run:

    $ make

then run it with:

    $ ./blade

- Windows

Just grab the exe file from the win/ directory and run it on a command prompt, you can do this by typing __Windows + R__ and then typing __cmd__, then drag the exe file into the command prompt and type __Enter__;

# Using blade

Blade is not a compiled language, and it can't read files by now, when you run blade, you should see a prompt that looks like this:

    blade -|>>> 

This means it worked, and now you can type some code in it, but, what kind of code? you will see now.

    blade -|>>> [id "Hello, Blade!"]
    got: [id "Hello, Blade!"]
    retrieved: Hello, Blade!

This is the id function, and the only thing it does is return exactly what it received, you can say: "but it isn't exact, the double quotes vanished!" Well, the quotes are used for another purpose, they actually prevent the quoted area from being evaluated, and then they send the unquoted area.

    [id "Hello, Blade!"] -> pass Hello, Blade! to the id function.

But, yes, id is kinda useless, let's actually __do__ something:

    blade -|>>> [+ 1 2]

So, here we call the "+" function with the arguments 1 and 2, and it's important to notice that the spaces between words/characters are __not optional__, spaces are used as separators, so you can't do this:

    [+12]

It will return an error because there is no function, but, you __can__ do this:

    [                    +           8       7]

It's not recommended tho, things like that can lead to unexpected stuff happening because the language is pretty new and unstable.

And, if you don't like the traditional lisp notation, you can do it the math way! although with some caveats, for example, you can do this:

    blade -|>>> [1 + 3]
    ...
    retrieved: 4

But you can't do that:

    blade -|>>> [1 + 5 - 2]

If you want to do 1 + 5 - 2, you should manually set the operator precedence using brackets:

    blade -|>>> [[1 + 5] - 2]
    ...
    retrieved: 4

Or you can do it the lispy way:

    blade -|>>> [- [+ 1 5] 2]
    ...
    retrieved: 4

The lispy way has some advantages when using operators on lists, because it makes the code more readable

    blade -|>>> [+ 1 2 3 4 5 6 7 8 9 10]
    ...
    
Instead of doing

    blade -|>>> [1 + 2 + 3 + 4 ...]
    ...

The way you do it doesn't actually matter, and btw, the math notation on the language is actually a bug in the parser.

> it's not a bug, it's a feature!

So, the mathematical functions are the following:

    * -> multiplication "[* 2 3] = 6" or "[2 * 3] = 6"
    / -> division "[/ 3 1] = 3" or "[3 / 1] = 3"
    + -> addition "[1 + 2]"
    - -> subtraction "[5 - 2]"

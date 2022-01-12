# Simon
This document treats several findings while researching SIMON.

## Plaintext generation
In theory, two methods of plaintext generation are prevalent.
These are
1. Counter mode: Simply take the index of your iterator as the plaintext
2. Random: generate a (pseudo-)random plaintext each time.

Both methods have their upsides and downsides, which we shortly discuss:
- Counter mode is faster than random: combined with a 32-round SIMON32 encryption, Random using Mersenne is ~33% slower.
- In counter mode, the Least Significant Bits (LSBs) are changed more often than the MSBs.
In Random, each bit is more or less random.
- Counter mode is sampling without replacement, whereas Random is sampling with replacement.

An approach that sits in the middle of these two modes is Prime Generation Mode.
In this approach, one takes a randomly sample starting point `s` and a fixed prime number P.
Hereafter, the plaintext `pt` in iteration `i` is computed as `pt_i := i * P + s mod N`, where `N` is the size of the sample space.
Mind you, `pt_i+1 = pt_i + P mod N`. 
Since N is usually `2^8, 2^16, 2^32 or 2^64` (i.e. a word size), the modulo step is superfluous, as the overflow already deals with this.
Thus, `pt_i+1 = pt_i + P`, making it a fast operation.

Discussion on Prime Generation Mode:
1. In 32-round SIMON32, it is about 4% slower than counter mode. Thus, much faster than random.
2. Still, sampling without replacement.
3. With large enough prime (p > (sample space size) / 2), bits are equally likely to be one or zero.
I.e. both LSBs and MSBs should be more uniformly distributed.
4. Mind you, this mode produces VERY uniform results, which might not actually be desirable.


## Code Optimization
In most situations, we wish to push an attack to the edge of what is possible, in attempt to extract as much information as possible.
This often results in performing large volumes of computations, such as generating many plain/cipher text pairs, or performing many analysis steps.
The trade-off with performing many computations is that this takes a lot of time.
In order to decrease the amount of time it takes to perform an attack, it is paramount that the attack code is RUN as efficiently as possible.
To achieve this, several techniques can be employed.

### Macro's and `inline` functions.
Separating your code into functions and subroutines make it better readable and changeable.
From a fast-execution point of view, this is however not ideal, as function calls often bring context-switches along.
These can be very costly.
To reap the benefits of writing code using functions, without the context switching, there are two options to consider: using macros and inline.

#### Macros.
In cpp, macro's have to be defined with `#define` and enable one-line functions to be written more concisely.
In fact, the compiler will automatically replace the macro with it (filled-in) definition.
Please bear in mind to place enough parentheses!

#### Inline functions.
If you have a subroutine that takes up multiple lines, an inline function may be a good alternative.
Here, one specifies a function can be inlined by prepending the word `inline` to the function header.
The compiler will use this and determine whether inlining your function would lead to an actual speed improvement.
Please bear in mind that some optimization flags automatically check whether functions that are not marked as `inline` can in fact be inlined.

## Compiler
Although efficient programming can achieve a lot of time gain, we can also use the compiler to optimize the translated byte-code of our program.

### Which compiler to use?
G++. Stock on Linus
Intel C++. Have not gotten it to work though... Could have benefits as I'm executing on an Intel platform.

### Which version to use
Interestingly, my speedtest is ~20% faster on g++-9.3, than it is on g++-10 or g++-11.
Also, g++-8 seems to be add odds with 9, maybe even slightly faster...
Curious.

### Optimization Flags (in g++)
In most compilers several optimization modes exist.
Without treading into too much detail, [g++ has the following modes](https://gcc.gnu.org/onlinedocs/gcc-11.2.0/gcc/Optimize-Options.html#Optimize-Options):
- O1, O2, O3: three speed optimization modes, each one more aggressive than the previous.
- OS: optimize executable size. Under the hood, it is O2, but without space increasing optimization options
- Ofast: disregards compliance.

In fact, each flag activates a set of optimization options simultaneously.
We will not tread in further detail about these individual optimization options.

Please bear in mind that these optimization options are not "Free speed"; they have their downsides.
In particular,
- fault tolerance goes out the windows, as memory buffers are optimized away. [source](?)
- the stability of your program diminishes, making the runtime less predictable. [source](personal experience)
- executable size is likely to grow.
- compilation time increases.

Curiously enough, in some situations the -OS flag led to faster computation times.
This could perhaps be a sign that something is off about the function structure in my files...

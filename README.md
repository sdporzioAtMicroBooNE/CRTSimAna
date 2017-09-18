# CRTSimAna

Development of LArSoft code for the analysis and calibration of the CRT simulation code.

In order to build it, you can use the same method as for any other repository (e.g. LArSim).


After having a local install of LArSoft, you can just go to your `src` directory and then run:

```
mrb g https://github.com/sdporzioAtMicroBooNE/CRTSimAna
```

Then `cd` to your build directory (you can do `cd ${MRB_BUILDDIR}`).

```
mrbsetenv
mrb i
```

P.S.:
You can run `mrb uc` if you are having problems with your CMake files not updating.

If you have more than one core, you can use the option `-j xx` with `xx` equal to the number of cores.
git
You can also use `ninja` if you have previously set it up, to make building much more efficient. E.g.:

```
mrb i --generator ninja -j 16
```
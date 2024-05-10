# Environment

Fork this repository and work in this fork. Sometimes you'll have to get some updates from the main repository (to receive new tasks). After cloning the fork to your pc locally execute
```
git remote add main_repo git@gitlab.spectral.tech:study/effective-cpp.git
# or git remote add main_repo https://gitlab.spectral.tech/study/effective-cpp.git
```

After that you can use `git pull main_repo main` to receive updates from the main repo.

Execute `git submodule update --init --recursive` inside `effective-cpp` directory after cloning.

All the tasks are designed in a way that you can do them locally on your pc.
However you need Linux or Mac OS in order to do that. Also make sure the architecture is x86-64 (check with `uname -a`), otherwise you won't be able
to complete some of the tasks. ~~Rest in peace if you have Apple M1.~~

<details>
<summary>For Apple M1 users</summary>

In order to test some tasks, that require x64 (mostly layout and gdb weeks), you can use docker like this
```bash
docker run -it --platform linux/amd64 ubuntu:22.04
```

And then run tests inside this container (you might want to install `build-essential` and `cmake` there). You don't need this
for the majority of tasks though.

</details>

Only a few number of tasks require the usage of C++20, otherwise C++17 is enough. In case you want a newer compiler you may install `clang-15` like this (on ubuntu):
```bash
cd /tmp && wget https://apt.llvm.org/llvm.sh && sed -i 's/http:/https:/g' ./llvm.sh && sudo chmod +x ./llvm.sh && sudo ./llvm.sh 15 all
```

On ubuntu you just may use
```
sudo apt-get install clang
```

And then use `clang++-15` (or whichever version you got) for compilation. You may also want to have a newer version of your standard library in case you want to use some c++20
features (although it's not required). Upgrading it is a complicated process and you can easily break your os if you don't know what are you doing exactly. So if you really want a newer version, I'd suggest using a docker container with a newer version of OS installed (ubuntu 22.04 for a example). You can check if you have a recent version by `#include <concepts>`.

Every task is located in its own directory (check https://gitlab.spectral.tech/study/effective-cpp/-/blob/main/tasks/multiplication for a example) and all the tasks belong to a single cmake project.

You only need to push the files you've been asked to modify in the task. You may also push anything else for your convinience, but only the mentioned files will be used in testing.

When you finish a task make sure the tests are passed in both `Release` and `ASAN` targets (check `multiplication` for the details). There are a few
exceptions to this rule, and they are always mentioned in a task's description.

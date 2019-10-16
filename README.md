# Sotosop
_a minified, but have more unique features than Photoshop_

## Compiling & Running

### Windows
1. Download and install Qt Creator with Qt Support (Hint: Do not install Android support and other uneeded support. Should only take ~10GB or something.)
2. run `qmake -project`
3. Add `gui widgets charts` to `QT` (under `INCLUDEPATH`). Should be something like this
```
    QT += gui widgets charts
```
3. Run it from Qt Creator

### Linux (Ubuntu & Debian-based distributions)

1. Make sure that `build-essential` is installed. On other distros, you may need to find it's counterparts.
```
sudo apt install build-essential
```

2. Install these Qt libraries with your package manager.
```
sudo apt install qtcreator qt5-default qt5-doc qtbase5-examples qtbase5-doc-html libqt5charts5 libqt5charts5-dev
```

3. Using terminal, navigate to the root of the project, and run this command. This will generate a Makefile that will be used to compile to an executable file.
```
qmake
```

4. Next, we compile the project using the Makefile.
```
make
```

5. The executable will pop up as `SotoShop`. Run that file!
Universal Scene Description Exporter for SketchUp
=================================================

This plug-in adds the ability to export [Pixar's Universal Scene
Description](http://openusd.org/docs/index.html) files from [SketchUp
Pro](https://www.sketchup.com). It has been tested with SketchUp Pro
2016, 2017, 2018 and 2019.

It adds three options to the **File**->**Export**->**3D Model** menu:

- Pixar USD binary File (*.usd)
- Pixar USD ASCII File (*.usda)
- Pixar USDZ  File (*.usdz)

There are also options on the export panel to conditionally export
**normals**, **curves**, **edges**, and **lines**, as well as the
ability to organize the USD as a **single file** or as a **set of
files** that reference each other.

The [**usdz**](https://graphics.pixar.com/usd/docs/Usdz-File-Format-Specification.html) files this exporter writes out takes care to write out in a way that is compatible with Apple's [ARKit 2](https://developer.apple.com/arkit/), which is more constrained than the general [**usdz** specification](https://graphics.pixar.com/usd/docs/Usdz-File-Format-Specification.html), but that support can be toggled on or off in the **Options...** dialog.

The exporter also leverages the new
[UsdPreviewSurface](https://graphics.pixar.com/usd/docs/UsdPreviewSurface-Proposal.html)
to support texture export from SketchUp.

Getting the Prebuilt Plugin for macOS
------------------------------

We are providing a pre-built version of the plug-in
[here](https://github.com/drwave/usd-sketchup/blob/master/USDExporter.plugin.zip). After
downloading, you'll want to copy it into the **PlugIns** directory
inside the SketchUp Pro app bundle.  You can do this from the Terminal
by the following. Note you will need to type an admin password, as
that directory is probably write-protected.

```
sudo cp -rf USDExporter.plugin /Applications/SketchUp\ 2018/SketchUp.app/Contents/PlugIns/
```

Once you have copied it there, you should see the 3 USD export options
under the **File**->**Export**->**3D Model** menu.

This build has been (lightly) tested on SketchUp 2019 (Version
19.2.221) on macOS 10.14.6, and was built with Xcode 10.3 (10G8)

Getting Help
------------

Need help understanding certain concepts in USD? See [Getting Help
with USD](http://openusd.org/docs/Getting-Help-with-USD.html) or visit
the [forum](https://groups.google.com/forum/#!forum/usd-interest).

If you are experiencing undocumented problems with the software,
please [file a
bug](https://github.com/drwave/usd-sketchup/issues/new).

Supported Platforms
-------------------

SketchUp Pro runs on macOS and Windows, but this plug-in is
**currently only supported on macOS**.

This plug-in was developed on macOS but care has been taken to make
sure that, as much as possible, it should be straightforward to port
to Windows. Both [SketchUp's SDK (*developer account
required*)](https://extensions.sketchup.com/en/developer_center/sketchup_sdk)
on Windows and macOS contain an example plug-in called `skp_to_xml`,
which this plug-in took inspiration from, so if someone wants to port
this to Windows they should just need to look in the
`SDK/samples/skp_to_xml/win/` and in the `USD SketchUp Mac` folder
here and do the equivalent for Windows.

Dependencies
------------

| Name | Version |
| ---- | ------- |
| [macOS](https://www.apple.com/mac/) | 10.13 or higher | 
| [SketchUp Pro](https://www.sketchup.com/download/all) | 2016 or higher | 
| [Xcode](https://developer.apple.com/xcode) | 9 or higher |
| [SketchUp SDK](https://extensions.sketchup.com/en/developer_center/sketchup_sdk) | recent|
| [USD](https://github.com/PixarAnimationStudios/USD) | 18.09 or higher |


Getting and Building the Code
-----------------------------


#### 1. Install prerequisites (see [Dependencies](#dependencies) for required versions)

Note: to build this plug-in you will need a [SketchUp Developer/Trimble account](https://developer.sketchup.com/en) and an [Apple Developer account](https://developer.apple.com/account/).

The rest of these instructions assume you have a
[Mac](https://www.apple.com/mac/) with
[Xcode](https://developer.apple.com/xcode) and [SketchUp
Pro](https://www.sketchup.com/download/all) installed.

#### 2. Download the USD source code

You can download source code archives from [GitHub](https://www.github.com/PixarAnimationStudios/USD) or use ```git``` to clone the repository.

```
git clone https://github.com/PixarAnimationStudios/USD
```

#### 3. Run the USD build script

##### MacOS:

In a terminal, run ```xcode-select``` to ensure command line developer
tools are installed. Then run the script. We recommend building
without Python, without imaging, and as a monolithic library that
includes tbb in it. The included Xcode project assumes that it has
been built that way and installed into
```/opt/local/USDForSketchUp```.

```
python USD/build_scripts/build_usd.py --build-args TBB,extra_inc=big_iron.inc --no-python --no-imaging --no-usdview --build-monolithic /opt/local/USDForSketchUp
```

#### 4. Download the SketchUp SDK

Once you login to the [Trimble/SketchUp developer
account](https://extensions.sketchup.com/en/developer_center/sketchup_sdk)
(*look in upper right corner of that linked page*), download the
[SDK](https://extensions.sketchup.com/en/developer_center/sketchup_sdk). Unzip
and install this somewhere on your machine, for example,
```~/SketchUpSDKs/SDK_Mac_2019-0-752_0```

#### 5. Download the USD SketchUp exporter plug-in source code

You can download source code archives from
[GitHub](https://www.github.com/drwave/usd-sketchup) or use ```git```
to clone the repository.

```
git clone https://github.com/drwave/usd-sketchup
```

At the top level of the repository, make a link to the SketchUp SDK
you installed. For example, if the SDK you downloaded was
```SDK_Mac_18-0-18665``` and you put it in a subdirectory off your
home directory called ```SketchUpSDKs```, you would do:

```
cd usd-sketchup
ln -s ~/SketchUpSDKs/SDK_Mac_2019-0-752_0 SDK_Mac
```
#### 6. Build the USD SketchUp exporter plugin

Launch Xcode on the [project
file](https://github.com/drwave/usd-sketchup/tree/master/usd-sketchup.xcodeproj). You
may need to fix up various things in the Xcode file that are specific
to your build if you have changed them (i.e. installed USD in a
different location, have a different version of SketchUp installed,
etc.).

You should create symlinks for SketchUp SDK (as `SDK_Mac`) and USD (as `USD`) in this folder.

For example:
```
ln -s /Users/<you>/SketchUpSDKs/SDK_Mac_2019-0-752_0 ./lib/SDK_Mac
ln -s /opt/local/USDForSketchUp ./lib/USD
```

The Xcode project assumes that you are building for **SketchUp Pro
2019**, and building the target will actually copy the resulting
```USDExporter.plugin``` into SketchUp Pro's app bundle in the PlugIns
directory, i.e. ```/Applications/SketchUp\
2019/SketchUp.app/Contents/PlugIns/```.

Initially, that directory will probably not be writable on your machine, so you may need to make it writable:

```
sudo chmod a+w /Applications/SketchUp\ 2019/SketchUp.app/Contents/PlugIns/
```

Copying the plug-in to the directory makes it very easy to debug the
plug-in, as you can launch SketchUp Pro from inside of Xcode, set
breakpoints in your plug-in, etc. Very handy when doing development.

#### 6. Build the USD SketchUp exporter command line app

Launch Xcode on the [project
file](https://github.com/drwave/usd-sketchup/tree/master/usd-sketchup.xcodeproj). You
may need to fix up various things in the Xcode file that are specific
to your build if you have changed them (i.e. installed USD in a
different location, have a different version of SketchUp installed,
etc.).


Contributing
------------

If you'd like to contribute to this USD plug-in (and we appreciate the
help!), please see the
[Contributing](http://openusd.org/docs/Contributing-to-USD.html) page
in the documentation for more information.

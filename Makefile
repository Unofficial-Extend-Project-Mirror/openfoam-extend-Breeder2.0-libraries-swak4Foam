default:
	echo "Use ./Allwmake"

cleanStuff:
	./Allwclean
	wcleanLnIncludeAll

prepareDebian:
	cd debian; ./prepareForPackaging.py

dpkg-only: cleanStuff prepareDebian
#	export DH_ALWAYS_EXCLUDE=.svn:.dep:.o; dpkg-buildpackage -us -uc
#	dpkg-buildpackage -k<PACKAGER_ID>
#	debuild -us -uc
	export DH_ALWAYS_EXCLUDE=.svn:.dep:.o; dpkg-buildpackage

source-dpkg: cleanStuff prepareDebian
	export DH_ALWAYS_EXCLUDE=.svn:.dep:.o; dpkg-buildpackage -S -sa

dpkg: dpkg-only

install:
	./downloadSimpleFunctionObjects.sh
	./Allwmake
	./copySwakFilesToSite.sh
	./removeSwakFilesFromLocal.sh

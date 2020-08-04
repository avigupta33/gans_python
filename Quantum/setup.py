from distutils.core import setup, Extension
import shutil, os, sys

module_name = "Quantum"

__location__ = os.path.realpath(
    os.path.join(os.getcwd(), os.path.dirname(__file__)))

# Remove existing builds already in build/lib
build_path = os.path.join(__location__, "build")
if os.path.isfile(build_path):
    for folder in os.listdir(build_path):
        if folder.startswith("lib."):
            folder_path = os.path.join(build_path, folder)
            for archive in os.listdir(folder_path):
                os.remove(os.path.join(folder_path, archive))

# Compile extension
module = Extension(module_name, sources=["src/quantum.c"])
setup(name="Quantum Matrix",
      version="1.0",
      author="Quinn Okabayashi",
      author_email="qokabay1@swarthmore.edu",
      description="Quinn's custom matrix library",
      ext_modules=[module])

# Remove existing archive files in project
for file in os.listdir(__location__):
    if file.startswith(module_name) and file.endswith(".so"):
            os.remove(os.path.join(__location__, file))

# Move compiled archive into proper folder
dest_path = __location__

for folder in os.listdir(build_path):
    if folder.startswith("lib."):
        folder_path = os.path.join(build_path, folder)
        for archive in os.listdir(folder_path):
            archive_path = os.path.join(folder_path, archive)
            shutil.move(archive_path, os.path.join(dest_path))

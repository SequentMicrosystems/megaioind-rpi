import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="megaioind",
    version="1.1.0",
    author="Sequent Microsystems",
    author_email="olcit@gmail.com",
    description="A set of functions to control Sequent Microsystems MegaIO-IND and MegaIO-BAS boards",
	license='MIT',
    url="https://www.sequentmicrosystems.com/megaio-ind.html",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 2/3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
)
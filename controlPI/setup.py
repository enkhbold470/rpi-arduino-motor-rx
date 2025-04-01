from setuptools import setup, find_packages

setup(
    name="controlPI",
    version="0.1.0",
    packages=find_packages(),
    install_requires=[
        "smbus2>=0.4.2",
    ],
    author="Your Name",
    author_email="your.email@example.com",
    description="Raspberry Pi to Arduino I2C Motor Controller",
    long_description=open("README.md").read(),
    long_description_content_type="text/markdown",
    url="https://github.com/yourusername/controlPI",
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: POSIX :: Linux",
    ],
    python_requires=">=3.6",
) 
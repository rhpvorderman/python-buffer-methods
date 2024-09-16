from setuptools import Extension, setup

setup(
    ext_modules=[
        Extension(name="_buffer_methods",
                  sources=["src/buffer_methods/_buffer_methodsmodule.c"]),
    ]
)

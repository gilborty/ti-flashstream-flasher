from setuptools import setup

setup(name='ti-flashstream-flasher',
      version='0.1',
      description='Flasher for TI FlashStream files. Specifically for the .bqfs and .dffs for the TI bq34z100-g1 fuel gauge',
      url='https://github.com/nerdgilbert/ti-flashstream-flasher',
      author='Gilbert Montague',
      author_email='gilbert@openrov.com',
      license='MIT',
      packages=['argparse', 'os', 'smbus', 'sys', 'time']
      )

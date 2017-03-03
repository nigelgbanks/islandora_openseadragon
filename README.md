# Islandora OpenSeadragon [![Build Status](https://travis-ci.org/Islandora/islandora_openseadragon.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_openseadragon)

# Introduction

A Djatoka TileSource for Seadragon

Based in spirit from the JS component of Kevin Clarke's [FreeLib-Djatoka](https://github.com/ksclarke/freelib-djatoka)

Instead of "synthesizing" the info for DZI, we create the path to access Djatoka directly, and obtain different regions for the tiles.

Reverse proxy config: We make the assumption that we (reverse) proxy Djatoka, to fix the same-origin issue.

For Apache, with Drupal running on the same box as Apache, a couple lines like:

```
ProxyPass /adore-djatoka http://localhost:8080/adore-djatoka
ProxyPassReverse /adore-djatoka http://localhost:8080/adore-djatoka
```

in the Apache config somewhere (either the main apache.conf, httpd.conf, or in and arbitrarily named `*.conf` in your Apache's conf.d directory should suffice to establish the reverse proxy.

In Debian derived systems one will need to create location entries for each proxy or remove the Deny from All in mod_proxy's conf file.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [OpenSeadragon](http://openseadragon.github.io/releases/openseadragon-bin-0.9.129.zip)
* [Islandora Paged Content](https://github.com/Islandora/islandora_paged_content/) (Conditional: should not require any additional actions from the user as the solution packs that use the feature requiring the islandora_paged_content module include it in their depency lists.)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

[Download](http://openseadragon.github.io/releases/openseadragon-bin-0.9.129.zip) and install the Openseadragon library to your sites/libraries folder, or run `drush openseadragon-plugin`. Openseadragon 0.9.129 is known to work well with Islandora.

Note: If you use the Drush command, it is advisable to Move (not copy) the install script to your `.drush` folder and run it.

## Configuration

### Djatoka Image Server
Set the paths for 'Djatoka server base URL' and configure OpenSeadradon in Administration » Islandora » OpenSeadragon (admin/islandora/module).

![Configuration](https://camo.githubusercontent.com/c1bf991b5cc758a4420444564a91b286007e6f6e/687474703a2f2f692e696d6775722e636f6d2f4e6566597169432e706e67)

If you have an *existing* install it's required to update Openseadragon to it's latest version. You can do this quickly 
with the provided Drush command.

```bash
drush openseadragonplugin
```

### Loris Image Server

The [Loris Image server](https://github.com/loris-imageserver/loris) currently only works when configured to directly 
communicate with Fedora. As such it does not work with Islandora's authentication system, meaning it can only be used to 
display public datastreams, or it can be configured to give full access to show anything in the system. This does not 
give any user the ability to access any datastream however, just the JPEG generated for display in the browser, not the 
original.

Additionally there are issues with it's caching system in that it needs to be manually flushed please read the project 
[documentation](https://github.com/loris-imageserver/loris/blob/development/doc/cache_maintenance.md) for more 
information.

Provided below is instructions for installing loris on Ubuntu 14.04, the commands / packages may change slightly 
depending on your setup.

#### Create loris user
```bash
useradd -d /var/www/loris2 -s /sbin/false loris
```

#### Setup locale

```bash
locale-gen "en_US.UTF-8"
update-locale LC_ALL="en_US.UTF-8" LANG="en_US.UTF-8"
dpkg-reconfigure locales
echo "export LC_ALL=\"en_US.UTF-8\"" >> /root/.bashrc
echo "LC_ALL=\"en_US.UTF-8\"" >> /etc/environment
echo "LANG=\"en_US.UTF-8\"" >> /etc/environment
```

#### Update packages and install tools
```bash
sudo apt-get update -y
sudo apt-get install -y \
            apache2 \
            curl \
            git \
            libapache2-mod-wsgi \
            libfreetype6 \
            libfreetype6-dev \
            libjpeg8 \
            libjpeg8-dev \
            liblcms2-2 \
            liblcms2-dev \
            liblcms2-utils \
            libtiff5-dev \
            python-dev \
            python-pip \
            python-setuptools \
            unzip \
            zlib1g-dev
```

#### Enable Apache Headers Module.
```bash
a2enmod headers expires
```

#### Install Kakadu
```bash
curl -o /usr/local/lib/libkdu_v74R.so -L https://github.com/loris-imageserver/loris/raw/development/lib/Linux/x86_64/libkdu_v74R.so
chmod 755 /usr/local/lib/libkdu_v74R.so
curl -o /usr/local/bin/kdu_expand -L https://github.com/loris-imageserver/loris/raw/development/bin/Linux/x86_64/kdu_expand
chmod 755 /usr/local/bin/kdu_expand
ln -s /usr/lib/`uname -i`-linux-gnu/libfreetype.so /usr/lib/
ln -s /usr/lib/`uname -i`-linux-gnu/libjpeg.so /usr/lib/
ln -s /usr/lib/`uname -i`-linux-gnu/libz.so /usr/lib/
ln -s /usr/lib/`uname -i`-linux-gnu/liblcms.so /usr/lib/
ln -s /usr/lib/`uname -i`-linux-gnu/libtiff.so /usr/lib/
echo "/usr/local/lib" >> /etc/ld.so.conf && ldconfig
```

#### Install Python and Loris dependencies.
```bash
pip install --upgrade pip
pip install Werkzeug configobj Pillow ordereddict requests mock responses
```

#### Clone Loris git repository (we currently recommend the development branch).
```bash
git clone https://github.com/loris-imageserver/loris.git /opt
```

#### Install Loris.
```bash
cd /opt/loris
./setup.py install
```

#### Setup image cache, with correct permissions.
```bash
mkdir /usr/local/share/images/loris
chown -R loris:loris /etc/loris2
chown -R loris:loris /usr/local/share/images/
```

#### Configure Apache
Choose the right number of process/threads/maximum requests for your needs.

Where you add these lines will vary depending on your sites setup, but will likely reside in
_/etc/apache2/sites-enabled/000-default.conf_:
```
AllowEncodedSlashes On
WSGIDaemonProcess loris2 user=loris group=loris processes=10 threads=15 maximum-requests=10000
WSGIScriptAlias /loris /var/www/loris2/loris2.wsgi
WSGIProcessGroup loris2
```

#### Config Loris
Refer to the project [documentation](https://github.com/loris-imageserver/loris/blob/development/doc/configuration.md) 
for more information. If you don't provide a user name and password the image server can only access public datastreams.

Modify _/etc/loris2/loris2.conf_, such that it contains the following:
```
[resolver]
    impl = 'loris.resolver.SimpleHTTPResolver'
    source_prefix='http://example.com:8080/fedora/objects/'
    source_suffix='/content'
    cache_root='/usr/local/share/images/loris'
    user='fedoraAdmin'
    pw='PASSWORD'
    uri_resolvable=False
    head_resolvable=True
```

#### Restart Apache
```bash
sudo apachectl restart
```

Finally set the paths for 'IIIF server base URL' and configure OpenSeadradon in Administration » Islandora » OpenSeadragon (admin/islandora/module).

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Open+Seadragon)

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Nigel Banks](https://github.com/nigelgbanks)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)

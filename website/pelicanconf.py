#!/usr/bin/env python
# -*- coding: utf-8 -*- #
from __future__ import unicode_literals

AUTHOR = u'Guy Rutenberg'
SITENAME = u'Open Yahtzee'
SITEURL = ''

PATH = 'content'

TIMEZONE = 'Asia/Jerusalem'

DEFAULT_LANG = u'en'

PAGE_PATHS = ['wiki']
PAGE_URL = 'wiki/{slug}/'
PAGE_SAVE_AS = 'wiki/{slug}/index.html'

# Feed generation is usually not desired when developing
FEED_ALL_ATOM = None
CATEGORY_FEED_ATOM = None
TRANSLATION_FEED_ATOM = None
AUTHOR_FEED_ATOM = None
AUTHOR_FEED_RSS = None

# Blogroll
LINKS = (('Browse Source', 'https://github.com/guyru/openyahtzee'),
         ('Commit Log', 'https://github.com/guyru/openyahtzee/commits/master'),
         ('Bug Tracker', 'https://github.com/guyru/openyahtzee/issues'),
         )

TAGS_SAVE_AS = ''
TAG_SAVE_AS = ''

# Social widget
# SOCIAL = (('You can add links in your config file', '#'),
#           ('Another social link', '#'),)
SOCIAL = False

DISPLAY_TAGS_ON_SIDEBAR = False

DEFAULT_PAGINATION = False

# Uncomment following line if you want document-relative URLs when developing
#RELATIVE_URLS = True

#SITELOGO = 'images/openyahtzee_logo_cropped.png'
SITELOGO = 'images/openyahtzee.png'
SITELOGO_SIZE = '60px';
#HIDE_SITENAME = True

THEME = './pelican-bootstrap3/'
BOOTSTRAP_THEME = 'journal'

STATIC_PATHS = ['images', 'extra/CNAME']
EXTRA_PATH_METADATA = {'extra/CNAME': {'path': 'CNAME'}}

GOOGLE_ANALYTICS_UNIVERSAL = 'UA-1882923-1'
GOOGLE_ANALYTICS_UNIVERSAL_PROPERTY = 'auto'

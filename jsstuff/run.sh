#!/usr/bin/env bash

BIN=/Users/chris/mozilla-central/obj-x86_64-apple-darwin18.0.0/dist/bin/
RUN_MOZILLA=$BIN/run-mozilla.sh
XPCSHELL=$BIN/xpcshell

# /Users/chris/Documents/Contracting/mozilla/testWebsites/verify.js
$RUN_MOZILLA $XPCSHELL $1 $2


# BIN=/Users/chris/mozilla-central/obj-x86_64-apple-darwin18.0.0/dist/bin/run-mozilla.sh /Users/chris/mozilla-central/obj-x86_64-apple-darwin18.0.0/dist/bin/xpcshell
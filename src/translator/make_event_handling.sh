#!/bin/bash
sed 's/^/strm << "/;s/$/\\n";/' $1 > $2


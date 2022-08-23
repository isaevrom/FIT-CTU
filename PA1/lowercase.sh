for f in *; do git mv "$f" "`echo $f | tr "[:upper:]" "[:lower:]"`"; done

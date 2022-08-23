for f in *; do git mv "$f" "$f.tmp"; git mv "$f.tmp" "`echo $f | tr "[:upper:]" "[:lower:]"`"; done

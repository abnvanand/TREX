for file in *.cpp *.h; do
  cat header.txt "$file" > tempfile; mv tempfile "$file"
done

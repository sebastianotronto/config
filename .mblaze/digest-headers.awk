#/^--- --- --- [0-9]/,/^$/ {
/^Received: /,/^$/ {
	if ($1 == "From:" || $1 == "Subject:" || $1 == "Date:" || 
	    $1 == "from:" || $1 == "subject:" || $1 == "date:" ||
	    $1 == "FROM:" || $1 == "SUBJECT:" || $1 == "DATE:" ||
	    $1 == "---" || NF == 0)
		print;
		next
}
{ print }

## nm_otool

recode the nm (with no options) and the otool command (exactly the same as otool -t)

### ft_nm
<b>ft_nm</b> - display name list (symbol table) <br />
<b>options</b>: <br />
because I use them: <br />
-g  Display only global (external) symbols. <br />
-j  Just display the symbol names (no value or type). <br />
-o  Prepend file or archive element name to each output line, rather than only once. <br />
-u  Display only undefined symbols.<br />
-U  Don't display undefined symbols. <br />

### ft_otool
<b>ft_otool</b> - object file displaying tool <br />
<b>options</b>: <br />
-a  Display the archive header, if the file is an archive. <br />
-t  Display the contents of the (\__TEXT,\__text) section. (default) <br />
-d  Display the contents of the (\__DATA,\__data) section. <br />

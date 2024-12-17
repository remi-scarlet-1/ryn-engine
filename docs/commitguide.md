# Commit formatting
Detailed explenation on how commits should be formatted.

<pre>
Bad commit> 'updated README.md'  
Good commit>  'docs: update README with developer tips'  
</pre>

A useful tip is thinking of the commit message as what it will do for the repo.  
Examples... This commit will:  
- 'docs: update README.md to add developer tips'.  
- 'feat: add ProgramController class'

## Formatting
<pre> Type(Scope): Subject </pre>  

Type:
<pre>
 feat		- A new feature  
 fix		- A bug fix  
 docs		- Documentation only changes  
 style  	- Changes that do not affect the meaning of the code (white-space, format, etc.)  
 refactor 	- A code change that neither fixes a bug nor adds a feature  
 test  		- Adding missing tests or correcting existing ones  
 chore  	- Changes to the build process or auxilary tools and libraries  
 perf  		- A code change that improves performance  
 build  	- Changes that affect the build system  
 temp  		- temporary commit that wont be included in change logs  
</pre>
	
Scope:
<pre>
Optional, can be anything specifying the scope of the commit.  
For example project, namespace, class, etc.  
</pre>

Subject:
<pre>
Brief summary of the change in simple present. Not capitalized. No period at the end.  
</pre>
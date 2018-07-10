#!/usr/bin/ruby

#puts "args: #{ARGV.inspect}"

dst = ARGV[0]
src = ARGV[1]

skip_lite = "#ifndef LITE\n"
skip_else = "#else  // LITE\n"
skip_end = "#endif  // LITE\n"


def file_to_array filename
	array_line = []  

	if File.exist? filename
	  File.foreach(filename) do |line|
	      array_line.push line
	  end
	end

	array_line
end



lines = file_to_array src

File.open(dst, 'w') { |f|
	do_write = true

	lines.each { |l|
		is_ifdef = false

		if do_write
			if l == skip_lite
				do_write = false
				is_ifdef = true
			elsif l == skip_end
				is_ifdef = true
			end
		else
			if l == skip_end || l == skip_else
				do_write = true
				is_ifdef = true
			end
		end

		if do_write && !is_ifdef
			f.write l
		end
	}
}

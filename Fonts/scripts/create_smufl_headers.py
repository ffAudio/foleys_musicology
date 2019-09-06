#!/usr/bin/python

import json

with open('../include/smufl.h', 'w') as header:
    with open('../SMuFL/metadata/glyphnames.json') as glyph_file:
        data = json.load(glyph_file)
        keys = sorted (data.keys())

        header.write('// This is a generated file\n\n')

        header.write('#pragma once\n\n')

        header.write('namespace smufl\n{\n\n')

        header.write('enum Glyph\n{\n')
        for glyph in keys:
            header.write('  G' + glyph + ',\n')

        header.write('  LAST_GLYPH\n };\n\n')

        header.write('static int codepoint[] =\n{\n')
        count = 0
        for glyph in keys:
            header.write('  ' + data[glyph]['codepoint'].replace("U+", "0x") + ', ')
            count += 1
            if count > 8:
                header.write('\n')
                count = 0

        header.write('};\n\n')

        header.write('static std::string glyphIDs[] =\n{\n')
        for glyph in keys:
            header.write('  \"' + glyph + '\",\n')

        header.write('  \"LAST_GLYPH\" };\n\n')

        header.write('static std::string glyphDescription[] =\n{\n')
        for glyph in keys:
            header.write('  \"' + data[glyph]['description'].encode('utf-8').replace("\"", "\\\"") + '\",\n')

        header.write('  \"LAST_GLYPH\" };\n\n')

        header.write('\n}; // namespace smufl\n')


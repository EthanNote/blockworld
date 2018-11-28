def gen_str_var(vname, vstring):
    l=[]
    l.append('const char* %s = \n'%(vname))
    for t in vstring.strip().split('\n'):
        l.append('"%s\\n"'%(t))
        l.append('\n')
    return l

def code_gen(code_list):
    result=[]
    for c in code_list:
        result+=gen_str_var(c["var_name"], c["var_string"])
        result.append(';\n\n')
    return result

def code_replace(target_file, line_list):
    f=open(target_file, 'r')
    lines=f.readlines()
    f.close()

    begin=-1
    end=-1
    for i, l in enumerate(lines):
        if begin<0 and l.strip() == "#define INSERT_BEGIN":
            begin=i
        elif l.strip() == "#define INSERT_END":
            end=i
    
    if begin<0 or end<0 or begin>=end:
        return lines
    output_list=[]
    output_list+=lines[:begin+1]
    output_list+=line_list
    output_list+=lines[end:]
    return output_list

code_list=[
    {"var_name":"shadow_map_vs", "var_string":open('shadow_map.vs', 'r').read()},
    {"var_name":"shadow_map_fs", "var_string":open('shadow_map.fs', 'r').read()},
    {"var_name":"depth_gray_vs", "var_string":open('depth_gray.vs', 'r').read()},
    {"var_name":"depth_gray_fs", "var_string":open('depth_gray.fs', 'r').read()},
    {"var_name":"quad_vs", "var_string":open('quad.vs', 'r').read()},
    {"var_name":"quad_fs", "var_string":open('quad.fs', 'r').read()},
    {"var_name":"depth_texture_vs", "var_string":open('depth_texture.vs', 'r').read()},
    {"var_name":"depth_texture_fs", "var_string":open('depth_texture.fs', 'r').read()},
]

def run(fname):
    lines=code_gen(code_list)
    gen=code_replace(fname, lines)
    open(fname, 'w').write(''.join(gen))

run('shader.c')
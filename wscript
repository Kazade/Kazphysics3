#!/usr/bin/env python

def options(opt):
    opt.load('compiler_c compiler_cxx')
    
def configure(cnf):
    cnf.load('compiler_c compiler_cxx')
    cnf.check(features='cxx cxxprogram', lib=['unittest++'], uselib_store='UNITTEST')
    cnf.check_cfg(path='sdl-config', args='--cflags --libs', package='', uselib_store='SDL')
    cnf.check(lib=['GL', 'GLU'], args='--cflags --libs', package='', uselib_store='GL')
    cnf.env.append_value('CCFLAGS', '-Wall -g')
    cnf.env.append_value('CXXFLAGS', ['-O0', '-g'])

def build(bld):
    kazmath_sources = """
src/kazmath/vec2.c
src/kazmath/mat3.c
src/kazmath/utility.c
"""
    
    bld(features='c cxx cxxstlib cstlib', 
        source='src/world.cpp src/object.cpp src/triangle.cpp src/utils/gl_utils.cpp' + kazmath_sources, 
        target='kazphysics3', 
        linkflags=['-g'], 
        cflags=['-fPIC', '-Wall'],
        includes="./src"
    )
    bld(features='c cxx cxxprogram', source='tests/test_world_basic.cpp', target='test_world_basic', use=['UNITTEST','kazphysics3'], lib=['dl'])
    bld(features='c cxx cxxprogram', source='tests/play_test.cpp tests/kaztimer/kaztimer.cpp', target='play_test', use=['kazphysics3', 'SDL', 'GL'], lib=['dl'], linkflags=['-g'], cflags=['-Wall'])

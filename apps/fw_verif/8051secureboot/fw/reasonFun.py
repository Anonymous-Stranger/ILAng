# python scripts for reasoning mmio function body.

import os
import sys
import argparse
import util

def reasonFun (funName, filePath, fileSuff):
    fileName = '%s/%s.%s' % (filePath, funName, fileSuff)
    if not os.path.isfile (fileName):
        print 'File ', fileName, ' not exist.'
        return

    lines = util.getLineFromFile (fileName)

    entryFileName = '%s/%s_entry.%s' % (filePath, funName, fileSuff)
    entryFile = open (entryFileName, 'w')
    exitFileName  = '%s/%s_exit.%s' % (filePath, funName, fileSuff)
    exitFile  = open (exitFileName, 'w')

    # find out entry memory
    firstSelect = ''
    selectPattern = '(select '
    for line in lines:
        if selectPattern in line:
            firstSelect = line
            break

    elements = firstSelect.split()
    entryMem = elements[1]

    # copy the entry part into the new file
    for line in lines:
        entryFile.write (line)
        if 'true' in line:
            break
    entryFile.write ('\t\t(= %s entryMem)\n' % entryMem)
    entryFile.close()
            
    # find out exit memory
    lastStore = ''
    prevLine  = ''
    prevStore = ''
    storePattern = '(store '
    for line in lines:
        if storePattern in line:
            lastStore = line
            prevStore = prevLine
        else:
            prevLine = line

    elements = prevStore.split()
    exitMem = elements[1]
    
    # copy the exit part into the new file
    exitFile.write ('\t\t(= %s exitMem)\n' % exitMem)
    cnt = 0
    for line in lines:
        if cnt > 0 or lastStore in line:
            cnt += 1
        if cnt >= 4:
            exitFile.write (line)
    exitFile.close()

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument ("funName", type=str,
                         help="funxtion name")
    parser.add_argument ("--path", type=str, default='info',
                         help="file path")
    parser.add_argument ("--suffix", type=str, default='smt2', 
                         help="file suffix")
    args = parser.parse_args()

    reasonFun (args.funName, args.path, args.suffix)

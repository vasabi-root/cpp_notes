def recursive_letters(lst: [], mask, antiLst = []):
    assert len(mask) == len(antiLst)
    for i, letter in enumerate(lst):
        j = mask.index('-')
        
        if letter not in antiLst[j]:
            nLst = list(lst)
            nLst.remove(letter)
            nMask = list(mask)
            nMask[j] = lst[i]
            
            recursive_letters(nLst, nMask, antiLst)
        
    if len(lst) == 1:
        j = mask.index('-')
        mask[j] = lst[0]
        print(''.join(mask))
        
def main():
    recursive_letters('асл+', '---о-', ['', 'л', 'с', '', 'а'])

if __name__ == '__main__':
    main()
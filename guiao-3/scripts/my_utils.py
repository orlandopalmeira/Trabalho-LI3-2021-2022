
def date_equal(d1, m1, y1, d2, m2, y2) -> bool:
    return y1 == y2 and m1 == m2 and d1 == d2

def date_lower(d1, m1, y1, d2, m2, y2) -> bool:
    if y1 > y2: return False
    elif y1 == y2 and m1 > m2: return False
    elif y1 == y2 and m1 == m2 and d1 > d2: return False
    elif y1 == y2 and m1 == m2 and d1 == d2: return False
    else: return True

def date_greater(d1, m1, y1, d2, m2, y2) -> bool:
    return not(date_lower(d1, m1, y1, d2, m2, y2)) and not(date_equal(d1, m1, y1, d2, m2, y2))

def date_between(dl, ml, yl, d, m, y, dg, mg, yg) -> bool:
    return (date_greater(d,m,y,dl,ml,yl) and date_lower(d,m,y,dg,mg,yg)) or date_equal(d,m,y,dl,ml,yl) or date_equal(d,m,y,dg,mg,yg)

def date_between_arr(datel, date, dateg) -> bool:
    """
    Datas inseridas no formato YYYY/MM/DD
    """
    return date_between(datel[2],datel[1],datel[0],date[2],date[1],date[0],dateg[2],dateg[1],dateg[0])

def date_equal_arr(date1, date2) -> bool:
    """
    Datas inseridas no formato YYYY/MM/DD
    """
    return date_equal(date1[2],date1[1],date1[0],date2[2],date2[1],date2[0])

def date_greater_arr(date1, date2) -> bool:
    """
    Datas inseridas no formato YYYY/MM/DD
    """
    return date_greater(date1[2],date1[1],date1[0],date2[2],date2[1],date2[0])

def date_lower_arr(date1, date2) -> bool:
    """
    Datas inseridas no formato YYYY/MM/DD
    """
    return date_lower(date1[2],date1[1],date1[0],date2[2],date2[1],date2[0])
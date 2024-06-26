# Брезенхем
Растеризация линии с тангенсом угла наклона от 0 до 1.

$$ y=kx+b $$ 
$$ y=\frac{dy}{dx}x+b $$ 
$$ y*dx=dy*x+b*dx $$ 
$$ x*dy - y*dx + b*dx = 0 $$ 
$$ f(x, y) = A*x + B*y + C = 0 $$
$$ A = dy $$
$$ B = -dx $$
$$ C = b*dx $$

Фишка в том, что если точка `(x, y)` принадлежит прямой, то `f = 0`. Если точка ближе к Ox -- то `f > 0`, если к Oy -- `f < 0`.

Пусть есть точка $(x_0, y_0)$, тогда чему равна y-координата для точки с $x=x_0+1$???  
Учитывая, что алгоритм работает только для `k in [0..1]`, новый y будет иметь координату либо $y_0$, либо $y_0+1$. Выбираем ту, что ближе к прямой.  
Как это понять? Посчитать значение $f(x_0+1, y_0+0.5)$. Если `f >= 0`, то выбираем $y_0+1$, иначе -- $y_0$

## Integer impl
Считать каждый раз $f(x_0+1, y_0+0.5)$ запарно, но есть выход -- считать разность между $f(x_0+1, y_0+0.5)$ и $f(x_0, y_0)$:
$$ D = f(x_0+1, y_0+0.5) - f(x_0, y_0) $$
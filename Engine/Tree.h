#include "pch.h"
#include "BaseGridSpace.h"

using namespace Microsoft::WRL;

class Tree : public BaseGridSpace
{
public:
	Tree();
	Tree(int row, int column, float x, float y, ComPtr<ID2D1SolidColorBrush> brush);
	~Tree();

	void Draw(ComPtr<ID2D1DeviceContext1> context);

protected:

private:
};
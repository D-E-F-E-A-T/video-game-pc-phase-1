#include "pch.h"
#include "BaseGridSpace.h"

using namespace Microsoft::WRL;

class Water : public BaseGridSpace
{
public:
	Water();
	Water(int row, int column, float x, float y, ComPtr<ID2D1SolidColorBrush> brush);
	~Water();

	void Draw(ComPtr<ID2D1DeviceContext1> context);

protected:

private:
};
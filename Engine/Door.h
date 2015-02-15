#include "pch.h"
#include "BaseGridSpace.h"

using namespace Microsoft::WRL;

class Door : public BaseGridSpace
{
public:
	Door();
	Door(int row, int column, float x, float y, ComPtr<ID2D1SolidColorBrush> brush);
	~Door();

	void Draw(ComPtr<ID2D1DeviceContext1> context);

protected:

private:
};
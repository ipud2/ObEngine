//Author : Sygmei
//Key : 976938ef7d46c286a2027d73f3a99467bcfa8ff0c1e10bd0016139744ef5404f4eb4d069709f9831f6de74a094944bf0f1c5bf89109e9855290336a66420376f

#include "ObEngine.hpp"

void LoadErrors()
{
	vili::LoadErrors("Data/Errors.vili");
}

int main(int argc, char** argv)
{
	obe::Functions::Run::Parser runParser(argv, argc);
	std::string startMode = runParser.getArgumentValue("-mode");
	std::cout << "Running ObEngine using mode : " << startMode << std::endl;

	obe::Coord::UnitVector::Init(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
	obe::Coord::WorldStruct* wVec;
	obe::Coord::UnitVector::Init(wVec);
	obe::Coord::ViewStruct* vVec;
	obe::Coord::UnitVector::Init(vVec);

	wVec->w = 3840;
	wVec->h = 3240;

	vVec->w = 960;
	vVec->h = 540;
	vVec->x = 2880;
	vVec->y = 1620;

	obe::Coord::UnitVector uv_a(384, 1296, obe::Coord::WorldUnits);
	std::cout << uv_a.to<obe::Coord::ViewPercentage>() << std::endl;
	std::cout << uv_a.to<obe::Coord::ViewPixels>() << std::endl;
	std::cout << uv_a.to<obe::Coord::ViewUnits>() << std::endl;
	std::cout << uv_a.to<obe::Coord::WorldPercentage>() << std::endl;
	std::cout << uv_a.to<obe::Coord::WorldPixels>() << std::endl;
	std::cout << uv_a.to<obe::Coord::WorldUnits>() << std::endl;

	vili::DataParser testvec;
	testvec.parseFile("testvec2.vili", true);
	std::cout << testvec->at<vili::BaseAttribute>("Miam", "pos", "x")->get<double>() << std::endl;
	std::cout << testvec->at<vili::BaseAttribute>("Polygones", "polygone1", "unit", "unit")->get<std::string>() << std::endl;

	std::cout << std::endl;
	std::cout << uv_a
		.to<obe::Coord::ViewPixels>()
		.to<obe::Coord::ViewUnits>()
		.to<obe::Coord::WorldPercentage>()
		.to<obe::Coord::WorldPixels>()
		.to<obe::Coord::WorldUnits>()
		.to<obe::Coord::ViewPercentage>()
		.to<obe::Coord::ViewPixels>() << std::endl;


	std::ofstream out("debug.log");
	std::streambuf *coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(out.rdbuf());

	std::ofstream out_err("errors.log");
	std::streambuf *cerrbug = std::cerr.rdbuf();
	std::cerr.rdbuf(out_err.rdbuf());

	LoadErrors();
	obe::System::MountPaths();

	if (startMode == "edit")
	{
		std::string editMapName = obe::Modes::chooseMapMenu();
		if (editMapName != "")
			obe::Editor::editMap(editMapName);
	}
	else if (startMode == "play")
		obe::Modes::startGame("");
	else if (startMode == "toolkit")
		obe::Modes::startToolkitMode();
	else
		obe::Modes::startGame("");
	
	return 0;
}
#pragma once


namespace Core::Scene
{
		struct Wall
		{
			union
			{
				struct
				{
					Vec3 p1;
					Vec3 p2;
					Vec3 p3;
					Vec3 p4;
				};
				Vec3 p[4];
			};
			Vec3 normal;
		};

		void init();
		void shutdown();
		void render();
		void addWall(Vec3 p1, Vec3  p2, Vec3  p3, Vec3  p4);
		void loadScene(const String& filePath);
		void saveScene(const String& filePath);
		void checkForSceneSwitch();
		DynArr<Ref<Wall>>& getWallArray();
		String& getLoadedSceneName();
}
import unreal

def check_uasset_bindings():
    # 애셋 레지스트리 가져오기
    asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
    
    # 실제 경로 지정
    path = '/Game'  # 여기를 실제 경로로 수정하세요
    
    print(f"Checking uasset files in {path}...")
    
    # 해당 경로의 모든 애셋 가져오기
    assets = asset_registry.get_assets_by_path(path, recursive=True)
    
    if len(assets) == 0:
        print(f"No assets found in {path}. Check if the path is correct.")
        return
    
    print(f"Found {len(assets)} assets.")
    
    # 문제 있는 파일 목록
    problematic_assets = []
    
    # uasset 파일만 확인
    uasset_count = 0
    for asset in assets:
        try:
            # 파일 경로 가져오기
            asset_path = None
            package_name = None
            
            # 패키지 이름 가져오기
            if hasattr(asset, 'package_name'):
                package_name = asset.package_name
            
            # 애셋 이름 가져오기
            asset_name = asset.asset_name if hasattr(asset, 'asset_name') else "Unknown"
            
            # 애셋 클래스 가져오기
            asset_class = asset.asset_class if hasattr(asset, 'asset_class') else "Unknown"
            
            # 전체 경로 구성
            if package_name:
                asset_path = f"{package_name}"
            
            has_problem = False
            problem_desc = ""
            
            # 파일 확장자 확인
            if asset_path and unreal.EditorAssetLibrary.does_asset_exist(asset_path):
                uasset_count += 1
                print(f"\nUAsset: {asset_name}")
                print(f"  Path: {asset_path}")
                print(f"  Class: {asset_class}")
                
                # 블루프린트인 경우 클래스 바인딩 정보 출력
                if asset_class == 'Blueprint':
                    try:
                        blueprint = unreal.EditorAssetLibrary.load_asset(asset_path)
                        if blueprint:
                            # 부모 클래스 확인
                            parent_class = blueprint.get_editor_property('parent_class')
                            if parent_class:
                                print(f"  Parent Class: {parent_class.get_name()}")
                                print(f"  Class Bound: Yes")
                            else:
                                print(f"  Parent Class: None")
                                print(f"  Class Bound: No")
                                has_problem = True
                                problem_desc = "Missing parent class"
                        else:
                            print("  Failed to load blueprint")
                            has_problem = True
                            problem_desc = "Could not load blueprint"
                    except Exception as e:
                        print(f"  Error loading blueprint: {e}")
                        has_problem = True
                        problem_desc = f"Error: {str(e)}"
                else:
                    # 다른 유형의 uasset 파일 처리
                    try:
                        asset_obj = unreal.EditorAssetLibrary.load_asset(asset_path)
                        if asset_obj:
                            class_name = asset_obj.get_class().get_name()
                            print(f"  Asset Type: {class_name}")
                            print(f"  Class Bound: Yes")
                        else:
                            print("  Class Bound: No")
                            has_problem = True
                            problem_desc = "Asset could not be loaded"
                    except Exception as e:
                        print(f"  Error loading asset: {e}")
                        print("  Class Bound: Unknown")
                        has_problem = True
                        problem_desc = f"Error: {str(e)}"
                
                # 문제가 있는 경우 목록에 추가
                if has_problem:
                    problematic_assets.append({
                        "name": asset_name,
                        "path": asset_path,
                        "class": asset_class,
                        "problem": problem_desc
                    })
        except Exception as e:
            print(f"Error processing asset: {e}")
            problematic_assets.append({
                "name": asset_name if 'asset_name' in locals() else "Unknown",
                "path": asset_path if 'asset_path' in locals() else "Unknown",
                "class": asset_class if 'asset_class' in locals() else "Unknown",
                "problem": f"Processing error: {str(e)}"
            })
    
    print(f"\nTotal uasset files found: {uasset_count}")
    
    # 문제가 있는 파일 요약 출력
    print("\n========== PROBLEMATIC ASSETS SUMMARY ==========")
    if problematic_assets:
        print(f"Found {len(problematic_assets)} problematic assets:")
        for i, asset in enumerate(problematic_assets, 1):
            print(f"\n{i}. {asset['name']}")
            print(f"   Path: {asset['path']}")
            print(f"   Class: {asset['class']}")
            print(f"   Problem: {asset['problem']}")
    else:
        print("No problematic assets found. All assets have proper class bindings.")

# 함수 실행
check_uasset_bindings()
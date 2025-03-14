import unreal
import sys

def check_uasset_bindings(search_path=None, recursive=True, show_all=True):
    """
    uasset 파일의 클래스 바인딩 정보를 검사하는 함수
    
    Args:
        search_path (str): 검색할 경로 (기본값: None, 입력하지 않으면 사용자에게 요청)
        recursive (bool): 하위 폴더까지 검색할지 여부 (기본값: True)
        show_all (bool): 모든 파일을 출력할지 또는 문제가 있는 파일만 출력할지 여부 (기본값: True)
    """
    # 애셋 레지스트리 가져오기
    asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
    
    # 경로가 제공되지 않은 경우 사용자에게 요청
    if not search_path:
        try:
            # 편집기 유틸리티 라이브러리가 있는 경우 사용
            selected_folder = unreal.EditorUtilityLibrary.pick_folder()
            if selected_folder:
                search_path = selected_folder
            else:
                print("No folder selected. Using '/Game' as default.")
                search_path = '/Game'
        except:
            # 대화상자를 표시할 수 없는 경우 기본 경로 사용
            print("Could not display folder selection dialog. Using '/Game' as default.")
            search_path = '/Game'
    
    print(f"Checking uasset files in {search_path}...")
    print(f"Options: recursive={recursive}, show_all={show_all}")
    
    # 해당 경로의 모든 애셋 가져오기
    assets = asset_registry.get_assets_by_path(search_path, recursive=recursive)
    
    if len(assets) == 0:
        print(f"No assets found in {search_path}. Check if the path is correct.")
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
                
                # 모든 파일을 출력하는 경우에만 정보 표시
                if show_all:
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
                                if show_all:
                                    print(f"  Parent Class: {parent_class.get_name()}")
                                    print(f"  Class Bound: Yes")
                            else:
                                if show_all:
                                    print(f"  Parent Class: None")
                                    print(f"  Class Bound: No")
                                has_problem = True
                                problem_desc = "Missing parent class"
                        else:
                            if show_all:
                                print("  Failed to load blueprint")
                            has_problem = True
                            problem_desc = "Could not load blueprint"
                    except Exception as e:
                        if show_all:
                            print(f"  Error loading blueprint: {e}")
                        has_problem = True
                        problem_desc = f"Error: {str(e)}"
                else:
                    # 다른 유형의 uasset 파일 처리
                    try:
                        asset_obj = unreal.EditorAssetLibrary.load_asset(asset_path)
                        if asset_obj:
                            if show_all:
                                class_name = asset_obj.get_class().get_name()
                                print(f"  Asset Type: {class_name}")
                                print(f"  Class Bound: Yes")
                        else:
                            if show_all:
                                print("  Class Bound: No")
                            has_problem = True
                            problem_desc = "Asset could not be loaded"
                    except Exception as e:
                        if show_all:
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
            if show_all:
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


# 간단한 인수 파서
def parse_args():
    # 기본값 설정
    args = {
        'search_path': None,
        'recursive': True,
        'show_all': True
    }
    
    # 인수 처리를 시도
    try:
        # unreal.sys 모듈이 있다면 사용
        if hasattr(unreal, 'sys') and hasattr(unreal.sys, 'argv'):
            argv = unreal.sys.argv
        else:
            # 일반 sys 모듈 사용
            argv = sys.argv
        
        # 인수 파싱
        for arg in argv:
            if arg.startswith("-path="):
                args['search_path'] = arg.split("=")[1]
            elif arg == "-nonrecursive":
                args['recursive'] = False
            elif arg == "-problemsonly":
                args['show_all'] = False
    except:
        # 인수 파싱에 실패하면 기본값 사용
        print("Failed to parse arguments. Using default values.")
    
    return args


# 사용 예시 및 도움말 출력
def print_usage():
    print("\nUsage Examples:")
    print("1. Basic usage (will show folder selection dialog):")
    print("   check_uasset_bindings()")
    print("\n2. With specific path:")
    print("   check_uasset_bindings('/Game/Characters')")
    print("\n3. With all options:")
    print("   check_uasset_bindings('/Game/Characters', recursive=False, show_all=False)")


# 메인 함수
def main():
    args = parse_args()
    check_uasset_bindings(args['search_path'], args['recursive'], args['show_all'])
    # print_usage()


# 직접 실행 시 main() 함수 호출
if __name__ == "__main__" or True:  # 에디터에서 실행할 때도 작동하도록
    main()
file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

set(CMAKE_BUILD_TYPE Debug)

add_library(spa
    # current directory
    ${srcs}
    ${headers}

    # lexer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer/Lexer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer/Lexer.cpp

    # ast library
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/AstTypes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/AstTypes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/AstLibrary.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/AstLibrary.cpp

    # error library
    ${CMAKE_CURRENT_SOURCE_DIR}/src/error/InputError.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/error/InputError.cpp

    # cfg library
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cfg/CfgBuilder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cfg/CfgBuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cfg/CfgTypes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cfg/CfgTypes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cfg/CfgBipBuilder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cfg/CfgBipBuilder.cpp

    # design extractor
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/DesignExtractor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/DesignExtractor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/SemanticErrorsValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/SemanticErrorsValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/FollowsExtractor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/FollowsExtractor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/ParentExtractor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/ParentExtractor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/UsesExtractor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/UsesExtractor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/ModifiesExtractor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/ModifiesExtractor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/NextExtractor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/NextExtractor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/CallsExtractor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/CallsExtractor.cpp

    # frontend/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/Parser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/Parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/Token.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/Token.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/StringMatcher_Impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/StringMatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/ErrorMessages.cpp

    # frontend manager
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/FrontendManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/FrontendManager.cpp

    # pkb
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/relationships/Follows.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/relationships/Follows.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/relationships/Parent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/relationships/Parent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/relationships/Modifies.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/relationships/Modifies.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/relationships/Uses.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/relationships/Uses.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/relationships/Next.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/relationships/Next.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/relationships/NextBip.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/relationships/NextBip.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/relationships/Calls.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/relationships/Calls.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/tables/Tables.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/tables/Tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/PKB.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/PKB.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/PkbTypes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/PKBUtils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/PKBUtils.cpp

    # pql/preprocessor
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/AbstractQuery.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/AbstractQuery.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/AqTypes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/AqTypesUtils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/AqTypesUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/Attribute.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/Attribute.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/Clause.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/Clause.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/ClauseVector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/ClauseVector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/DeclarationTable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/DeclarationTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/DesignEntity.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/DesignEntity.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/DesignEntity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/DesignEntityType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/Errorable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/Errorable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/ExpressionSpec.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/PatternClause.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/PatternClause.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/Preprocessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/Preprocessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/ExpressionSpec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/QueryErrorType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/Reference.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/Reference.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/Relationship.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/Relationship.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/ResultSynonym.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/ResultSynonym.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/ResultSynonymVector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/ResultSynonymVector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/SuchThatClause.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/SuchThatClause.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/preprocessor/WithClause.cpp

    # pql/evaluator
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/Evaluator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/Evaluator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/EvaluatorUtils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/EvaluatorUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/ResultsTable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/ResultsTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/RelationshipsGraph.cpp

    # pql/evaluator/relationships
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/CallsEvaluator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/CallsEvaluator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/CacheSet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/CacheSet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/CacheTable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/CacheTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/FollowsEvaluator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/FollowsEvaluator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/ModifiesEvaluator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/ModifiesEvaluator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/NextEvaluator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/NextEvaluator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/ParentEvaluator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/ParentEvaluator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/RelationshipsUtil.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/SuchThatEvaluator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/SuchThatEvaluator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/UsesEvaluator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/UsesEvaluator.cpp

    # pql/evaluator/relationships/affects
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/affects/AffectsEvaluator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/affects/AffectsEvaluator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/affects/AffectsEvaluatorFacade.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/relationships/affects/AffectsEvaluatorFacade.cpp

    # pql/evaluator/pattern
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/pattern/AssignMatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/pattern/AssignMatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/pattern/IfMatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/pattern/IfMatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/pattern/PatternMatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/pattern/PatternMatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/pattern/PatternMatcherUtil.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/pattern/PatternMatcherUtil.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/pattern/WhileMatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/pattern/WhileMatcher.cpp

    # pql/evaluator/attribute
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/attribute/AttributeMap.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/attribute/AttributeMap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/attribute/WithUnifier.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/evaluator/attribute/WithUnifier.cpp

    # pql/projector
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/projector/FormattedQueryResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/projector/FormattedQueryResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/projector/Projector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/projector/Projector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/projector/RawQueryResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/projector/RawQueryResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/projector/QueryResultFormatType.h

    # pql/optimiser
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/optimiser/Optimiser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/optimiser/Optimiser.cpp

    # pql manager
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/PqlManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/PqlManager.cpp)

# flags for debug
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(spa PRIVATE "-Wall;-Wextra;-pedantic;-D_GLIBCXX_DEBUG")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(spa PRIVATE "-Wall;-Wextra;-pedantic")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Bill Gates created c0v1d to s3ll vacc1n3sz!!!1!
endif ()

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


"""Helper functions to consolidate different CMSSW configs"""
import FWCore.ParameterSet.Config as cms

def addSkim(process, isData=False):
    print "Adding Skim step."
    process.jetFilter = cms.EDFilter("GenJetFilter",
        srcJets = cms.InputTag("ak4PFJetsCHS"),
        # srcJets = cms.InputTag("patJets"),
        # additionalCut = cms.string(""),
        additionalCut = cms.string("abs(eta) < 2.5 && pt > 50.0"),
        jetCuts = cms.VPSet(
            cms.PSet(
                minPt = cms.double(400.0),
                maxEta = cms.double(2.5),
                stringCut = cms.string(""),
                ),
            cms.PSet(
                minPt = cms.double(200.0),
                maxEta = cms.double(2.5),
                stringCut = cms.string(""),
                ),
            cms.PSet(
                minPt = cms.double(125.0),
                maxEta = cms.double(2.5),
                stringCut = cms.string(""),
                ),
            cms.PSet(
                minPt = cms.double(50.0),
                maxEta = cms.double(2.5),
                stringCut = cms.string(""),
                ),
        )
    )
    return cms.Sequence(process.jetFilter)


def addAnalyze(process, isData=False, sample=''):
    process.GenAnalyzer = cms.EDAnalyzer('EMJGenAnalyzer',
        srcJets = cms.InputTag("jetFilter", "selectedJets"),
    )

    process.GenAnalyzer = cms.EDFilter('EMJGenAnalyzer',
        srcJets = cms.InputTag("jetFilter", "selectedJets"),
        idbg = cms.untracked.int32(100),
    )

    # return cms.Sequence(process.emergingJetAnalyzer+process.emJetAnalyzer)
    return cms.Sequence(process.GenAnalyzer)


